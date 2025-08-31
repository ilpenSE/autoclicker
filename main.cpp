#include <QApplication>
#include <QFile>
#include <QLocale>
#include <QMessageBox>
#include <QTranslator>

#include "LoggerStream.h"
#include "appdatamanager.h"
#include "clickengine.h"
#include "consts.h"
#include "languagemanager.h"
#include "logger.h"
#include "macromanager.h"
#include "mainwindow.h"
#include "settingsmanager.h"
#include "thememanager.h"
#include "versionmanager.h"

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  // Version initializationları
  VersionManager::instance().initVer(APP_VERSION);

  // ikon ayarla
  app.setWindowIcon(QIcon(":/assets/icons/icon.ico"));

  // AppData verilerini kontrol et
  if (!AppDataManager::instance().ensureAppDataFolderExists()) {
    lnerr() << "(From main) AppData cannot be created or found.";
    QMessageBox::critical(NULL, "Error", "AppData cannot be created or found.");
    return -1;
  }
  if (!AppDataManager::instance().ensureDefaultAssets()) {
    lnerr() << "(From main) Default assets cannot be downloaded.";
    QMessageBox::critical(nullptr, "Error", "Default assets cannot be downloaded. Check your internet connection.");
    return -1;
  }

  // ayarları yükle
  // BURDA, APPDATA/ROAMING/AutoClicker2/settings.json'dan ayarları çekicek
  QString settingsPath = AppDataManager::instance().settingsFilePath();

  bool ok = false;
  QJsonObject settings =
      SettingsManager::instance().loadSettings(settingsPath, ok);

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
    fswrn() << "Settings file was deleted or corrupted, created one.";
  } else {
    // Dosya mevcutsa SADECE eksik ayarları ekle, mevcut ayarları değiştirme!
    bool needsUpdate = false;

    // Sadece eksik ayarları kontrol et
    QJsonObject defaults = SettingsManager::instance().defaultSettings();
    for (auto it = defaults.begin(); it != defaults.end(); ++it) {
      if (!settings.contains(it.key())) {
        settings[it.key()] = it.value();
        needsUpdate = true;
        swrn() << "Missing key added: " << it.key();
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

  // Burada dili yüklerken dosyadan okunan değeri kullan
  QString savedLanguage = settings["Language"].toString("en_US");

  lninfo() << "(From main) Loading language from settings: " << savedLanguage;

  if (!LanguageManager::instance().loadLanguage(savedLanguage)) {
    lnerr() << "(From main) Language cannot be loaded: " << savedLanguage;
    // Fallback olarak English'i dene
    if (!LanguageManager::instance().loadLanguage("en_US")) {
      QMessageBox::critical(NULL, "Error", "Language cannot be loaded.");
      return -1;
    }
  }

  // Theme dosyası kontrolü
  QString themesPath = AppDataManager::instance().appFolderPath() + "/themes.json";

  bool themesOk = false;
  QJsonObject themes =
      SettingsManager::instance().loadSettings(themesPath, themesOk);

  if (!themesOk) {
    // Varsayılan themes.json oluştur
    QJsonObject defaults;
    defaults["dark"] = "Fluent Dark";
    defaults["light"] = "Fluent Light";

    SettingsManager::instance().saveSettings(themesPath, defaults);
    fswrn() << "Themes file was deleted or corrupted, created one.";
    themes = defaults;
  } else {
    // Eksik key varsa ekle
    bool needsUpdate = false;
    if (!themes.contains("dark")) {
      themes["dark"] = "Fluent Dark";
      needsUpdate = true;
    }
    if (!themes.contains("light")) {
      themes["light"] = "Fluent Light";
      needsUpdate = true;
    }

    if (needsUpdate) {
      SettingsManager::instance().saveSettings(themesPath, themes);
    }
  }

  // temayı yükle
  QString theme = settings["Theme"].toString("dark");
  if (!ThemeManager::instance().applyTheme(
          ThemeManager::instance().getVisibleName(theme))) {
    therr() << "(From main) Theme cannot be loaded, file name: " << theme
            << " visible name: "
            << ThemeManager::instance().getVisibleName(theme);
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
