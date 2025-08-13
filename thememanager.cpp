#include "thememanager.h"
#include <QStandardPaths>
#include "logger.h"
#include "appdatamanager.h"
#include <QApplication>

ThemeManager::ThemeManager(QObject* parent)
    : QObject(parent) {}

QString ThemeManager::themesDirPath() const {
    QString path = AppDataManager::instance().appFolderPath();
    QDir dir(path + "/themes");
    if (!dir.exists()) {
        dir.mkpath(".");
        Logger::instance().fsWarning("Themes folder not found, creating one...");
    }
    return dir.absolutePath();
}

QString ThemeManager::readQssFile(const QString& filePath) const {
    QFile file(filePath);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        Logger::instance().fsError("Theme file cannot be opened: " + filePath);
        return QString();
    }
    return QString::fromUtf8(file.readAll());
}

bool ThemeManager::applyTheme(const QString& themeName) {
    QString filePath = themesDirPath() + "/" + themeName + ".qss";
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
    Logger::instance().thInfo("Theme applied: " + themeName);
    return true;
}

QStringList ThemeManager::availableThemes() const {
    QDir dir(themesDirPath());
    QStringList themes;
    QStringList files = dir.entryList(QStringList() << "*.qss", QDir::Files);
    for (const QString& file : files) {
        QString thname = QFileInfo(file).baseName();
        themes << thname;
    }
    return themes;
}
