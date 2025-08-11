#include "mainwindow.h"
#include "languagemanager.h"
#include "settingsmanager.h"
#include "appdatamanager.h"
#include "logger.h"
#include "versionmanager.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Version initializationları
    VersionManager::instance().initVer("2.0-beta1");

    // AppData verilerini kontrol et
    if (!AppDataManager::instance().ensureAppDataFolderExists()) {
        Logger::instance().langError("(From main) AppData cannot be created or found.");
        QMessageBox::critical(NULL, "Error", "AppData cannot be created or found.");
        return -1;
    }

    // ayarları yükle
    // BURDA, APPDATA/ROAMING/AutoClicker2/settings.json'dan ayarları çekicek
    QString settingsPath = AppDataManager::instance().settingsFilePath();

    bool ok = false;
    QJsonObject settings = SettingsManager::instance().loadSettings(settingsPath, ok);

    if (!ok) {
        // Ayar dosyası yok veya bozuksa default oluşturup kaydet
        settings = SettingsManager::instance().defaultSettings();
        SettingsManager::instance().saveSettings(settingsPath, settings);
    }

    // Ayarları doğrula ve gerekirse düzelt
    bool changed = SettingsManager::instance().validateAndFixSettings(settings);

    if (changed) {
        SettingsManager::instance().saveSettings(settingsPath, settings);
    }

    // dili yükle
    if (!LanguageManager::instance().loadLanguage()) {
        Logger::instance().langError("(From main) Language cannot be loaded.");
        QMessageBox::critical(NULL, "Error", "Language cannot be loaded.");
        return -1;
    }

    MainWindow w(settings);
    w.show();
    return app.exec();
}
