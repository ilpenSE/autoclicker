// FIXME: Updater closing'te yüklediği installer'ı da kapatıyor
#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QProcess>
#include <QStandardPaths>
#include <QTimer>

#include "updatechecker.h"
#include "versioninfo.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include <shellapi.h>
#endif

void runInstallerWindows(const QString &installerPath) {
#ifdef Q_OS_WIN
  // Windows'ta ShellExecuteW kullan (Unicode desteği için)
  QString nativePath = QDir::toNativeSeparators(installerPath);

  SHELLEXECUTEINFOW sei{};
  sei.cbSize = sizeof(SHELLEXECUTEINFOW);
  sei.fMask = SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FLAG_NO_UI;
  sei.lpVerb = L"runas"; // admin
  sei.lpFile = reinterpret_cast<LPCWSTR>(nativePath.utf16());
  sei.lpParameters = nullptr;
  sei.nShow = SW_SHOWNORMAL;

  if (!ShellExecuteExW(&sei)) {
    DWORD error = GetLastError();
    qWarning() << "ShellExecuteEx failed with error code:" << error;

    sei.lpVerb = L"open";
    if (!ShellExecuteExW(&sei)) {
      error = GetLastError();
      qWarning() << "ShellExecuteEx (open) also failed with error code:"
                 << error;
    }
  } else {
    qDebug() << "Installer started successfully via ShellExecuteEx";
  }
#else
  qWarning()
      << "Windows-specific installer launching not available on this platform";
#endif
}

void runInstaller(const QString &installerPath) {
  QFileInfo fileInfo(installerPath);
  if (!fileInfo.exists() || fileInfo.size() == 0) {
    qWarning() << "Installer file invalid:" << installerPath;
    return;
  }

#ifdef Q_OS_WIN
  runInstallerWindows(installerPath);
#else
  QProcess::startDetached(installerPath);
#endif
}

void openMainApp() {
  QString appPath =
      QDir(QCoreApplication::applicationDirPath()).filePath("AutoClicker.exe");
  if (!QFile::exists(appPath)) {
    qWarning() << "Main application not found:" << appPath;
    return;
  }

  bool started = QProcess::startDetached(appPath);
  if (!started) {
    qWarning() << "Failed to start main application:" << appPath;
  } else {
    qDebug() << "Main application started:" << appPath;
  }
}

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);

  // Settings.json yolunu belirle
  QString roaming = qEnvironmentVariable("APPDATA");
  if (roaming.isEmpty()) {
    roaming = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  }
  QString settingsPath = QDir(roaming).filePath("AutoClicker2/settings.json");

  if (!VersionInfo::instance().loadFromSettings(settingsPath)) {
    qWarning() << "Cannot load version info.";
    openMainApp();
    return -1;
  }

  Version current = VersionInfo::instance().currentVersion();
  qDebug() << "Current version:" << current.toString();

  // Network manager
  QNetworkAccessManager networkManager;

  QObject::connect(
      &UpdateChecker::instance(), &UpdateChecker::updateFound,
      [&app, &networkManager]() {
        QString url = UpdateChecker::instance().latestInstallerUrl();
        qDebug() << "Update available! Downloading from:" << url;

        if (url.isEmpty()) {
          qWarning() << "Installer URL is empty!";
          openMainApp();
          app.quit();
          return;
        }

        // QNetworkRequest
        QNetworkRequest request{QUrl(url)};
        request.setAttribute(QNetworkRequest::RedirectPolicyAttribute,
                             QNetworkRequest::NoLessSafeRedirectPolicy);
        request.setRawHeader("User-Agent", "AutoClicker-Updater/1.0");

        QNetworkReply *reply = networkManager.get(request);

        QObject::connect(reply, &QNetworkReply::downloadProgress,
                         [](qint64 received, qint64 total) {
                           if (total > 0) {
                             int progress =
                                 static_cast<int>((received * 100) / total);
                             qDebug()
                                 << "Download progress:" << progress << "%";
                           }
                         });

        QObject::connect(reply, &QNetworkReply::finished, [reply, &app]() {
          if (reply->error() != QNetworkReply::NoError) {
            qWarning() << "Download failed:" << reply->errorString();
            reply->deleteLater();
            openMainApp();
            app.quit();
            return;
          }

          QByteArray data = reply->readAll();
          reply->deleteLater();

          if (data.isEmpty()) {
            qWarning() << "Downloaded data is empty!";
            openMainApp();
            app.quit();
            return;
          }

          QString tempDir =
              QStandardPaths::writableLocation(QStandardPaths::TempLocation);
          QString installerName =
              QString("AutoClicker2_Update_%1.exe")
                  .arg(
                      QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
          QString installerPath = QDir(tempDir).filePath(installerName);
          QDir().mkpath(tempDir);

          QFile file(installerPath);
          if (!file.open(QIODevice::WriteOnly)) {
            qWarning() << "Cannot create installer file:" << installerPath;
            openMainApp();
            app.quit();
            return;
          }

          file.write(data);
          file.flush();
          file.close();

          qDebug() << "Installer saved to:" << installerPath;
          runInstaller(installerPath);

          QTimer::singleShot(3000, [&app]() {
            qDebug() << "Updater closing...";
            app.quit();
          });
        });
      });

  QObject::connect(
      &UpdateChecker::instance(), &UpdateChecker::noUpdateFound, [&app]() {
        qDebug() << "No update available. Starting main application...";
        openMainApp();
        QTimer::singleShot(1000, [&app]() { app.quit(); });
      });

  QObject::connect(&UpdateChecker::instance(), &UpdateChecker::errorOccurred,
                   [&app](const QString &msg) {
                     qWarning() << "Update check error:" << msg;
                     openMainApp();
                     QTimer::singleShot(1000, [&app]() { app.quit(); });
                   });

  UpdateChecker::instance().checkForUpdates(current);

  return app.exec();
}
