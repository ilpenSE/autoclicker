#include <QApplication>
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
#include "updatedialog.h"

#ifdef Q_OS_WIN
#include <windows.h>
#include <shellapi.h>
#endif
#ifdef Q_OS_WIN
#include <TlHelp32.h>

bool isProcessRunning(const wchar_t* processName) {
    bool found = false;
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return false;
    PROCESSENTRY32W pe;
    pe.dwSize = sizeof(pe);
    if (Process32FirstW(snap, &pe)) {
        do {
            if (_wcsicmp(pe.szExeFile, processName) == 0) {
                found = true;
                break;
            }
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
    return found;
}
#endif

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

HANDLE g_installerProcess = nullptr;

void monitorInstallerProcess() {
#ifdef Q_OS_WIN
    if (!g_installerProcess) return;

    DWORD exitCode;
    if (GetExitCodeProcess(g_installerProcess, &exitCode)) {
        if (exitCode != STILL_ACTIVE) {
            qDebug() << "Installer process finished with exit code:" << exitCode;
            CloseHandle(g_installerProcess);
            g_installerProcess = nullptr;
            QTimer::singleShot(300, [](){
                QCoreApplication::quit();
            });

            return;
        }
    }

    // Installer hâlâ çalışıyor, 2 saniye sonra tekrar dene
    QTimer::singleShot(2000, monitorInstallerProcess);
#endif
}

void runInstaller(const QString &installerPath) {
    QFileInfo fileInfo(installerPath);
    if (!fileInfo.exists() || fileInfo.size() == 0) {
        qWarning() << "Installer file invalid:" << installerPath;
        return;
    }

#ifdef Q_OS_WIN
    SHELLEXECUTEINFOW sei;
    ZeroMemory(&sei, sizeof(sei));
    sei.cbSize = sizeof(sei);
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = nullptr;

    // Buradaki "runas" UAC penceresi açtırır
    sei.lpVerb = L"runas";
    sei.lpFile = (LPCWSTR)installerPath.utf16();
    sei.lpParameters = nullptr;
    sei.lpDirectory = nullptr;
    sei.nShow = SW_SHOWNORMAL;

    if (ShellExecuteExW(&sei)) {
        qDebug() << "Installer started with admin rights, monitoring process...";
        g_installerProcess = sei.hProcess;
        QTimer::singleShot(2000, monitorInstallerProcess);
    } else {
        DWORD err = GetLastError();
        qWarning() << "ShellExecuteEx (runas) failed with error:" << err;
    }
#else
    QProcess::startDetached(installerPath);
#endif
}


int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

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
          UpdateDialog *dlg = new UpdateDialog(nullptr);
          dlg->setAttribute(Qt::WA_DeleteOnClose); // kapandığında bellekten silinsin
          dlg->setModal(true);
          dlg->show();

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
                         [dlg](qint64 received, qint64 total) {
                             dlg->updateProgress(received, total);
                         });

        QObject::connect(reply, &QNetworkReply::finished, [reply, &app, dlg]() {
            if (reply->error() != QNetworkReply::NoError) {
                qWarning() << "Download failed:" << reply->errorString();
                reply->deleteLater();
                dlg->reject();
                openMainApp();
                app.quit();
                return;
            }

            QByteArray data = reply->readAll();
            reply->deleteLater();

            if (data.isEmpty()) {
                qWarning() << "Downloaded data is empty!";
                dlg->reject();
                openMainApp();
                app.quit();
                return;
            }

            QString tempDir =
                QStandardPaths::writableLocation(QStandardPaths::TempLocation);
            QString installerName =
                QString("AutoClicker2_Update_%1.exe")
                    .arg(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss"));
            QString installerPath = QDir(tempDir).filePath(installerName);
            QDir().mkpath(tempDir);

            QFile file(installerPath);
            if (!file.open(QIODevice::WriteOnly)) {
                qWarning() << "Cannot create installer file:" << installerPath;
                dlg->reject();
                openMainApp();
                app.quit();
                return;
            }

            file.write(data);
            file.flush();
            file.close();

            qDebug() << "Installer saved to:" << installerPath;

            dlg->accept();
            runInstaller(installerPath);
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
