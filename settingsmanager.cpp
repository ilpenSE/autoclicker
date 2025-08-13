#include "settingsmanager.h"
#include "logger.h"
#include "consts.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// Tüm ayarları kontrol eder ve düzeltir
void SettingsManager::validateAndFixSettings(QJsonObject &settingsObj) {
    QJsonObject defaults = defaultSettings();

    // Tanınmayan key’leri sil
    for (auto it = settingsObj.begin(); it != settingsObj.end();) {
        if (!defaults.contains(it.key())) {
            it = settingsObj.erase(it);
            Logger::instance().sWarning("Undefined key found: " + it.key() + ", deleting it.");
        } else {
            ++it;
        }
    }

    // Eksik değerleri default ile tamamla
    for (auto it = defaults.begin(); it != defaults.end(); ++it) {
        if (!settingsObj.contains(it.key())) {
            settingsObj[it.key()] = it.value();
            Logger::instance().sWarning("The key " + it.key() + " cannot be found, adding it");
        }
    }

    // DefaultHotkey kontrolü: örn. "Ctrl+F6", "Shift+Alt+X" gibi olmalı
    if (settingsObj.contains("DefaultHotkey") && settingsObj["DefaultHotkey"].isString()) {
        QString hotkey = settingsObj["DefaultHotkey"].toString();

        // Basit regex ile Ctrl, Shift, Alt içeren ve bir harf/fonksiyon tuşu kontrolü
        static const QRegularExpression hotkeyRegex("^(Ctrl\\+|Shift\\+|Alt\\+)*[A-Za-z0-9]+$");

        if (!hotkeyRegex.match(hotkey).hasMatch()) {
            // Hatalıysa default ile değiştir
            settingsObj["DefaultHotkey"] = defaults["DefaultHotkey"];
            Logger::instance().sWarning("Default Hotkey was provided wrong, defaulting it");
        }
    } else {
        settingsObj["DefaultHotkey"] = defaults["DefaultHotkey"];
        Logger::instance().sWarning("Default Hotkey was not provided, adding it.");
    }

    // Language kontrolü: sadece izin verilenler olmalı
    static const QStringList allowedLanguages = LanguageManager::instance().localeToLanguageMap.keys();
    if (settingsObj.contains("Language") && settingsObj["Language"].isString()) {
        QString lang = settingsObj["Language"].toString();
        if (!allowedLanguages.contains(lang)) {
            settingsObj["Language"] = defaults["Language"];
            Logger::instance().sWarning("Language was provided wrong, defaulting it");
        }
    } else {
        settingsObj["Language"] = defaults["Language"];
        Logger::instance().sWarning("Language was not provided, adding it.");
    }

    // Version kontrolü: consts.h'den APP_VERSION ile aynı olmalı
    if (settingsObj.contains("Version") && settingsObj["Version"].isString()) {
        QString verinfile = settingsObj["Version"].toString();
        if (verinfile != APP_VERSION) {
            settingsObj["Version"] = APP_VERSION;
            Logger::instance().sWarning("Updating settings file version from " + verinfile + " to " + APP_VERSION);
        }
    } else {
        settingsObj["Version"] = APP_VERSION;
        Logger::instance().sWarning("Settings file version cannot be found, setting it to " + APP_VERSION);
    }

    // ActiveMacros güvenlik filtresi (SQL injection gibi açıkları engeller)
    /*
     if (settingsObj["ActiveMacros"].isArray()) {
        QJsonArray filtered;
        const QJsonArray &macros = settingsObj["ActiveMacros"].toArray();


         * Burda makro ismi 1-50 karakter arasında olmalı
         * İsim şartları:
         * Türkçe karakterler dahil olmak üzere tüm büyük/küçük harfler
         * Rakamlar (0-9)
         * Alttan tire (_), Tire (-) ve Boşluk
         *
        static const QRegularExpression safeName("^[A-Za-zÇĞİÖŞÜçğıöşü0-9 _-]{1,50}$"); // sadece güvenli karakterler

        for (const auto &macroVal : macros) {
            if (macroVal.isString() && safeName.match(macroVal.toString()).hasMatch()) {
                filtered.append(macroVal);
            } else {
                Logger::instance().sWarning("This macro name isn't valid: " + macroVal.toString());
            }
        }
        settingsObj["ActiveMacros"] = filtered;
    } */
}

// Dosyadan ayarları okur, parse eder ve döner
// Başarılıysa ok true olur, aksi halde boş QJsonObject döner
QJsonObject SettingsManager::loadSettings(const QString& path, bool& ok)
{
    ok = false;
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Logger::instance().fsError("Settings file cannot be opened: " + path);
        return QJsonObject();
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
        Logger::instance().fsError("Settings file parse error: " + parseError.errorString());
        return QJsonObject();
    }

    ok = true;
    return doc.object();
}

// Ayarları dosyaya kaydeder
bool SettingsManager::saveSettings(const QString& path, const QJsonObject& settingsObj)
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        Logger::instance().fsError("Settings file cannot be written: " + path);
        return false;
    }

    QJsonDocument doc(settingsObj);
    file.write(doc.toJson(QJsonDocument::Indented));
    file.close();

    return true;
}
