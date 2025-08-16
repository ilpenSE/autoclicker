#include "mainwindow.h"
#include "languagemanager.h"
#include "settingsmanager.h"
#include "appdatamanager.h"
#include "logger.h"
#include "versionmanager.h"
#include "consts.h"
#include "thememanager.h"
#include "macromanager.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QMessageBox>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Version initializationları
    VersionManager::instance().initVer(APP_VERSION);

    // ikon ayarla
    app.setWindowIcon(QIcon(":/assets/icons/icon.ico"));

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

        // ÖNEMLİ: İlk kez çalışıyorsa sistem dilini kullan
        bool isFirstRun = true;
        if (isFirstRun) {
            settings["Language"] = LanguageManager::instance().getsyslang();
            settings["FirstRun"] = false;
        }

        SettingsManager::instance().saveSettings(settingsPath, settings);
        Logger::instance().fsWarning("Settings file was deleted or corrupted, created one.");
    } else {
        // ÖNEMLİ: Dosya mevcutsa SADECE eksik ayarları ekle, mevcut ayarları değiştirme!
        bool needsUpdate = false;

        // Sadece eksik ayarları kontrol et
        QJsonObject defaults = SettingsManager::instance().defaultSettings();
        for (auto it = defaults.begin(); it != defaults.end(); ++it) {
            if (!settings.contains(it.key())) {
                settings[it.key()] = it.value();
                needsUpdate = true;
                Logger::instance().sWarning("Missing key added: " + it.key());
            }
        }

        // Version güncelle
        if (settings["Version"].toString() != APP_VERSION) {
            settings["Version"] = APP_VERSION;
            needsUpdate = true;
        }

        // Sadece değişiklik varsa kaydet
        if (needsUpdate) {
            SettingsManager::instance().saveSettings(settingsPath, settings);
        }
    }

    // ÖNEMLİ: Burada dili yüklerken dosyadan okunan değeri kullan
    QString savedLanguage = settings["Language"].toString("en_US");
    Logger::instance().langInfo("(From main) Loading language from settings: " + savedLanguage);

    if (!LanguageManager::instance().loadLanguage(savedLanguage)) {
        Logger::instance().langError("(From main) Language cannot be loaded: " + savedLanguage);
        // Fallback olarak English'i dene
        if (!LanguageManager::instance().loadLanguage("en_US")) {
            QMessageBox::critical(NULL, "Error", "Language cannot be loaded.");
            return -1;
        }
    }

    // temayı yükle
    QString theme = settings["Theme"].toString("dark");
    if (!ThemeManager::instance().applyTheme(ThemeManager::instance().getVisibleName(theme))) {
        Logger::instance().thError("(From main) Theme cannot be loaded, file name: " + theme + " visible name: " + ThemeManager::instance().getVisibleName(theme));
        QMessageBox::critical(NULL, "Error", "Theme cannot be loaded.");
        return -1;
    }

    // makroyu database'ini yükle
    if (!MacroManager::instance().init()) {
        QMessageBox::critical(NULL, "Error", "Database cannot be opened!");
        return -1;
    }
    QVector<Macro> macros = MacroManager::instance().getAllMacros();

    MainWindow w(settings, macros);
    w.show();
    return app.exec();
}
