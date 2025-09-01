#pragma once
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>

struct Version {
  int major;
  int minor;
  int patch;
  int beta; // 0 = stable, >0 = beta number

  static Version fromString(const QString &str);
  bool operator<(const Version &other) const;
  QString toString() const;
};

class VersionInfo {
public:
  static VersionInfo &instance() {
    static VersionInfo _instance;
    return _instance;
  }

  Version currentVersion() const { return m_version; }
  QString currentVersionString() const { return m_version.toString(); }

  bool loadFromSettings(const QString &settingsPath);

private:
  VersionInfo() = default;
  Version m_version;
};
