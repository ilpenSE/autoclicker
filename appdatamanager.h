#ifndef APPDATAMANAGER_H
#define APPDATAMANAGER_H

#include "logger.h"

#include <QObject>
#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>

/*
 * singleton yapısı, normal classlardan farkı bu, program boyunca çalışır
 * ve aşağıdaki ayarlar sayesinde kopyalanamaz. Sadece 1 tane vardır.
 * Erişimi kolaydır, AppDataManager::instace() ile tüm slotlara sinyallere
 * ulaşılabilir. Ayrıca sinyal/slot yapısı bunlarda daha kolaydır
*/
class AppDataManager : public QObject {
    Q_OBJECT
public:
    static AppDataManager& instance()
    {
        static AppDataManager _instance;
        return _instance;
    }


    // fonksiyonlar
    bool ensureAppDataFolderExists();
    QString appFolderPath();

    // settings.json için
    bool checkSettingsFileExists();
    QString settingsFilePath();
    bool createSettingsFile();
    bool fixSettingsFile();

private:
    explicit AppDataManager(QObject *parent = nullptr) : QObject(parent) {}

    bool saveSettingsJson(const QJsonObject& obj) {
        QFile file(settingsFilePath());
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            qWarning() << "settings.json yazılamadı";
            return false;
        }
        QJsonDocument doc(obj);
        file.write(doc.toJson(QJsonDocument::Indented));
        file.close();
        return true;
    }

    // Copy ve assign engelle
    AppDataManager(const AppDataManager&) = delete;
    AppDataManager& operator=(const AppDataManager&) = delete;

    ~AppDataManager() override = default;
};

#endif // APPDATAMANAGER_H
