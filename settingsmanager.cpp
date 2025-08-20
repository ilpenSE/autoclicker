#include "settingsmanager.h"
#include "logger.h"
#include "consts.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// Tüm ayarları kontrol eder ve düzeltir - SADECE eksik olanları ekle, mevcut değerleri değiştirme!
void SettingsManager::validateAndFixSettings(QJsonObject &settingsObj) {
    QJsonObject defaults = defaultSettings();

    // Tanınmayan key'leri sil
    for (auto it = settingsObj.begin(); it != settingsObj.end();) {
        if (!defaults.contains(it.key())) {
            Logger::instance().sWarning("Undefined key found: " + it.key() + ", deleting it.");
            it = settingsObj.erase(it);
        } else {
            ++it;
        }
    }

    // ÖNEMLİ: SADECE eksik değerleri ekle, mevcut değerleri DOKUNMA!
    for (auto it = defaults.begin(); it != defaults.end(); ++it) {
        if (!settingsObj.contains(it.key())) {
            settingsObj[it.key()] = it.value();
            Logger::instance().sWarning("The key " + it.key() + " cannot be found, adding it");
        }
    }

    // Version kontrolü: sadece version güncelle, diğer hiçbir şeye dokunma
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

    Logger::instance().sInfo("Settings validated. Only missing keys added and version updated.");
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

    Logger::instance().fsInfo("Settings saved to: " + path);
    return true;
}
