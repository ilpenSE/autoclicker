#include "appdatamanager.h"
#include "logger.h"
#include "settingsmanager.h"

#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

QString AppDataManager::appFolderPath() {
#ifdef Q_OS_WIN
    QString roaming = qEnvironmentVariable("APPDATA");
    if (roaming.isEmpty()) {
        roaming = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    }
    return QDir(roaming).filePath("AutoClicker2");
#else
    QString base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    return QDir(base).filePath("AutoClicker2");
#endif
}

bool AppDataManager::ensureAppDataFolderExists() {
    QDir dir(appFolderPath());
    if (!dir.exists()) {
        if (!dir.mkpath(".")) {
            Logger::instance().fsError("Directory cannot be created: " + appFolderPath());
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
    return saveSettingsJson(defaultSettings);
}

bool AppDataManager::fixSettingsFile() {
    QFile file(settingsFilePath());
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "settings.json açılamadı, oluşturulacak";
        return createSettingsFile();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        qWarning() << "settings.json parse hatası, oluşturulacak";
        return createSettingsFile();
    }

    QJsonObject settingsObj = doc.object();
    bool changed = SettingsManager::instance().validateAndFixSettings(settingsObj);
    if (changed) {
        return saveSettingsJson(settingsObj);
    }
    return true;
}

