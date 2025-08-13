#ifndef THEMEMANAGER_H
#define THEMEMANAGER_H

#include <QObject>
#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include <QDebug>

class ThemeManager : public QObject {
    Q_OBJECT
public:
    static ThemeManager& instance()
    {
        static ThemeManager _instance;
        return _instance;
    }

    // fonksiyonlar
    bool applyTheme(const QString& themeName);
    QStringList availableThemes() const;
    QString themesDirPath() const;

private:
    QString readQssFile(const QString& filePath) const;
    explicit ThemeManager(QObject* parent = nullptr);
    ~ThemeManager() = default;

    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
};

#endif // THEMEMANAGER_H
