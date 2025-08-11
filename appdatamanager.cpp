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

bool AppDataManager::ensureAppDataFolderExists() {
    // AppData\Roaming yolu alınıyor
    QString roamingPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    // QStandardPaths::AppDataLocation zaten genellikle "Roaming/<OrgName>/<AppName>" verir,
    // Ama direkt "AutoClicker2" eklenebilir:
    QString appFolderPath = QDir(roamingPath).filePath("AutoClicker2");

    QDir dir(appFolderPath);
    if (!dir.exists()) {
        bool created = dir.mkpath(".");
        if (created) {
            Logger::instance().fsInfo("Directory created: " + appFolderPath);
        } else {
            Logger::instance().fsError("Directory cannot be created: " + appFolderPath);
        }

        return created;
    } else {
        Logger::instance().fsInfo("Directory already exists: " + appFolderPath);
        return true;
    }
}

// Ayar dosyası yolu
QString AppDataManager::settingsFilePath() {
    QString appDataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(appDataPath);
    if (!dir.exists()) dir.mkpath(".");
    return dir.filePath("settings.json");
}

// Dosya var mı?
bool AppDataManager::checkSettingsFileExists() {
    QFile file(settingsFilePath());
    return file.exists();
}

// Dosya oluştur (default içeriği SettingsManager'dan alabilirsin veya buraya koyabilirsin)
bool AppDataManager::createSettingsFile() {
    if (checkSettingsFileExists()) return true;

    // Default içeriği SettingsManager'dan al
    QJsonObject defaultSettings = SettingsManager::instance().defaultSettings();

    return saveSettingsJson(defaultSettings);
}

// Dosyayı onar / eksik alanları tamamla
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

    return true; // Sağlam ve değişiklik yok
}
