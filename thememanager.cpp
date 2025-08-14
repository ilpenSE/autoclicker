#include "thememanager.h"
#include <QStandardPaths>
#include "logger.h"
#include "appdatamanager.h"
#include <QApplication>
#include <QJsonObject>

// key: görünür isim, value: dosya ismi
QMap<QString, QString> themeMap;
QMap<QString, QString> reversedThemeMap;

ThemeManager::ThemeManager(QObject* parent)
    : QObject(parent) {
    loadThemesFromJson();
    reversedThemeMap = reverseThemeMap();
}

QString ThemeManager::themesDirPath() const {
    QString path = AppDataManager::instance().appFolderPath();
    QDir dir(path + "/themes");
    if (!dir.exists()) {
        dir.mkpath(".");
        Logger::instance().fsWarning("Themes folder not found, creating one...");
    }
    return dir.absolutePath();
}

QString ThemeManager::themesJsonPath() const {
    QString path = AppDataManager::instance().appFolderPath();
    return path + "/themes.json";
}

QString ThemeManager::readQssFile(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        Logger::instance().fsError("Theme file cannot be opened: " + filePath);
        return QString();
    }
    return QString::fromUtf8(file.readAll());
}

void ThemeManager::loadThemesFromJson() {
    themeMap.clear();

    QFile file(themesJsonPath());
    if (!file.exists()) {
        Logger::instance().fsError("themes.json not found: " + themesJsonPath());
        return;
    }

    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        Logger::instance().fsError("Cannot open themes.json: " + themesJsonPath());
        return;
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    if (!doc.isObject()) {
        Logger::instance().fsError("themes.json is not a valid JSON object.");
        // FIXME: appdatamanager'da themes.json kontrolleri yapılsın
        return;
    }

    QJsonObject obj = doc.object();
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        QString qssFile = it.key();
        QString visibleName = it.value().toString();
        themeMap[visibleName] = qssFile;
        Logger::instance().thInfo("Theme loaded, visible name: " + visibleName + " / file name: " + qssFile);
    }

    Logger::instance().thInfo("Loaded themes from themes.json");
}

bool ThemeManager::applyTheme(const QString& visibleName) {
    if (!themeMap.contains(visibleName)) {
        Logger::instance().thError("Theme not found: " + visibleName);
        return false;
    }

    QString qssFileName = themeMap[visibleName];
    QString filePath = themesDirPath() + "/" + qssFileName + ".qss";
    if (!QFile::exists(filePath)) {
        Logger::instance().fsError("Theme file cannot be found: " + filePath);
        return false;
    }

    QString qss = readQssFile(filePath);
    if (qss.isEmpty()) {
        Logger::instance().thError("QSS file is empty.");
        return false;
    }

    qApp->setStyleSheet(qss);
    Logger::instance().thInfo("Theme applied: " + visibleName);
    return true;
}

QMap<QString, QString> ThemeManager::reverseThemeMap() const {
    QMap<QString, QString> reversed;
    for (auto it = themeMap.constBegin(); it != themeMap.constEnd(); ++it) {
        reversed[it.value()] = it.key();
    }
    return reversed;
}

QStringList ThemeManager::availableThemes() const {
    return themeMap.keys();
}

QString ThemeManager::getVisibleName(const QString& filename) {
    return reversedThemeMap[filename];
}
