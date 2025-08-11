#include "settingsmanager.h"
#include "appdatamanager.h"
#include "logger.h"

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

// Tüm ayarları kontrol eder ve düzeltir
bool SettingsManager::validateAndFixSettings(QJsonObject& settingsObj)
{
    bool changed = false;

    const QJsonObject defaults = defaultSettings();

    // Language kontrolü
    QString lang = settingsObj.value("Language").toString();
    if (!supportedLanguages.contains(lang)) {
        Logger::instance().sWarning("Language value corrupted or out-of-support, setting it to default: " + lang);
        settingsObj["Language"] = defaults["Language"];
        changed = true;
    }

    // DefaultHotkey kontrolü
    QString hotkey = settingsObj.value("DefaultHotkey").toString();
    QRegularExpression hotkeyRegex("^(Shift \\+ |Ctrl \\+ |Alt \\+ )*(F[1-9][0-2]?)$");
    if (!hotkeyRegex.match(hotkey).hasMatch()) {
        Logger::instance().sWarning("DefaultHotkey corrupted, setting it to default: " + hotkey);
        settingsObj["DefaultHotkey"] = defaults["DefaultHotkey"];
        changed = true;
    }

    // FirstRun kontrolü
    if (!settingsObj.contains("FirstRun") || !settingsObj.value("FirstRun").isBool()) {
        Logger::instance().sWarning("FirstRun corrupted, settings it to default.");
        settingsObj["FirstRun"] = defaults["FirstRun"];
        changed = true;
    }

    // ActiveMacros kontrolü
    if (!settingsObj.contains("ActiveMacros") || !settingsObj.value("ActiveMacros").isArray()) {
        Logger::instance().sWarning("ActiveMacros corrupted, setting it to default.");
        settingsObj["ActiveMacros"] = defaults["ActiveMacros"];
        changed = true;
    }

    // Version kontrolü
    QString ver = settingsObj.value("Version").toString();
    if (ver.isEmpty()) {
        Logger::instance().sWarning("Version is empty, setting it to default.");
        settingsObj["Version"] = currentVersion;
        changed = true;
    }

    return changed;
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
