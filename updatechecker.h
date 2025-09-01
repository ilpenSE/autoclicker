#pragma once
#include "versioninfo.h"
#include <QNetworkAccessManager>
#include <QObject>
#include <QString>

class UpdateChecker : public QObject {
  Q_OBJECT
public:
  static UpdateChecker &instance() {
    static UpdateChecker _instance;
    return _instance;
  }

  void checkForUpdates(const Version &currentVersion);

  QString latestInstallerUrl() const { return m_latestUrl; }
  QString latestVersionString() const { return m_latestVersion.toString(); }
  bool updateAvailable() const { return m_updateAvailable; }

signals:
  void updateFound();
  void noUpdateFound();
  void errorOccurred(const QString &msg);

private:
  UpdateChecker() = default;
  QNetworkAccessManager m_networkManager;
  Version m_latestVersion;
  QString m_latestUrl;
  bool m_updateAvailable = false;
};
