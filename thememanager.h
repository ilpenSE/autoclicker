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
    bool applyTheme(const QString& visibleName);
    QStringList availableThemes() const;
    QString themesDirPath() const;
    QString themesJsonPath() const;
    void loadThemesFromJson();
    QMap<QString, QString> reverseThemeMap() const;
    QString getVisibleName(const QString& filename);
    QString getFileName(const QString& visibleName);

private:
    QString readQssFile(const QString& filePath) const;
    explicit ThemeManager(QObject* parent = nullptr);
    ~ThemeManager() = default;

    ThemeManager(const ThemeManager&) = delete;
    ThemeManager& operator=(const ThemeManager&) = delete;
};

#endif // THEMEMANAGER_H
