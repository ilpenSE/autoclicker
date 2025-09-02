#include "appdatamanager.h"

#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>

#include "consts.h"
#include "logger.h"
#include "settingsmanager.h"

QString AppDataManager::appFolderPath() {
#ifdef Q_OS_WIN
  QString roaming = qEnvironmentVariable("APPDATA");
  if (roaming.isEmpty()) {
    roaming = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  }
  return QDir(roaming).filePath("AutoClicker2");
#else
  QString base =
      QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  return QDir(base).filePath("AutoClicker2");
#endif
}

bool AppDataManager::ensureAppDataFolderExists() {
  QDir dir(appFolderPath());
  if (!dir.exists()) {
    if (!dir.mkpath(".")) {
      Logger::instance().fsError("Directory cannot be created: " +
                                 appFolderPath());
      return false;
    }
    Logger::instance().fsInfo("Directory created: " + appFolderPath());
  } else {
    Logger::instance().fsInfo("Directory already exists: " + appFolderPath());
  }

  // logs klasörü
  QString logsPath = dir.filePath("logs");
  QDir logsDir(logsPath);
  if (!logsDir.exists()) {
    if (logsDir.mkpath(".")) {
      Logger::instance().fsInfo("Logs folder created: " + logsPath);
    } else {
      Logger::instance().fsError("Logs folder cannot be created: " + logsPath);
    }
  }

  return true;
}

QString AppDataManager::settingsFilePath() {
  return QDir(appFolderPath()).filePath("settings.json");
}

bool AppDataManager::checkSettingsFileExists() {
  return QFile::exists(settingsFilePath());
}

bool AppDataManager::createSettingsFile() {
  if (checkSettingsFileExists()) return true;
  QJsonObject defaultSettings = SettingsManager::instance().defaultSettings();

  Logger::instance().fsInfo("Creating settings.json file");
  return saveSettingsJson(defaultSettings);
}

bool AppDataManager::fixSettingsFile() {
  QFile file(settingsFilePath());
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    Logger::instance().fsWarning(
        "Settings.json cannot be opened, will be created.");
    return createSettingsFile();
  }

  QByteArray data = file.readAll();
  file.close();

  QJsonParseError parseError;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
  if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
    Logger::instance().fsWarning("Settings.json parse error, will be created.");
    return createSettingsFile();
  }

  QJsonObject settingsObj = doc.object();
  SettingsManager::instance().validateAndFixSettings(settingsObj);

  return saveSettingsJson(settingsObj);
}

QString AppDataManager::assetUrl(const QString& relativePath) {
  return QString("https://raw.githubusercontent.com/%1/%2/%3/%4")
      .arg(GITHUB_USER)
      .arg(GITHUB_REPO)
      .arg(GITHUB_BRANCH)
      .arg(relativePath);
}

bool AppDataManager::ensureFileExists(const QString& relativePath) {
  QString localPath = QDir(appFolderPath()).filePath(relativePath);
  if (QFile::exists(localPath)) {
    return true;  // zaten var
  }

  Logger::instance().fsWarning("File missing, will try to download: " +
                               relativePath);

  // GitHub'dan indir
  QNetworkAccessManager mgr;
  QNetworkRequest req(QUrl(assetUrl(relativePath)));
  QNetworkReply* reply = mgr.get(req);

  QEventLoop loop;
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  if (reply->error() != QNetworkReply::NoError) {
    Logger::instance().fsError("Cannot download " + relativePath + ": " +
                               reply->errorString());
    reply->deleteLater();
    return false;  // internet yok veya dosya bulunamadı
  }

  QByteArray data = reply->readAll();
  reply->deleteLater();

  // Klasörü oluştur
  QDir().mkpath(QFileInfo(localPath).absolutePath());

  QFile f(localPath);
  if (f.open(QIODevice::WriteOnly)) {
    f.write(data);
    f.close();
    Logger::instance().fsInfo("Downloaded and saved: " + relativePath);
    return true;
  } else {
    Logger::instance().fsError("Cannot write file: " + localPath);
    return false;
  }
}

bool AppDataManager::ensureDefaultAssets() {
  // QSS dosyaları
  if (!ensureFileExists("themes/dark.qss")) return false;
  if (!ensureFileExists("themes/light.qss")) return false;

  return true;
}
