#include "updatechecker.h"
#include <QDebug>
#include <QEventLoop>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkReply>
#include <QNetworkRequest>

void UpdateChecker::checkForUpdates(const Version &currentVersion) {
  QString url = QString("https://api.github.com/repos/%1/%2/releases")
                    .arg("ilpenSE")
                    .arg("autoclicker");

  QNetworkRequest request(url);
  QNetworkReply *reply = m_networkManager.get(request);

  QEventLoop loop;
  QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
  loop.exec();

  if (reply->error() != QNetworkReply::NoError) {
    emit errorOccurred(reply->errorString());
    reply->deleteLater();
    return;
  }

  QByteArray data = reply->readAll();
  reply->deleteLater();

  QJsonParseError parseError;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
  if (parseError.error != QJsonParseError::NoError || !doc.isArray()) {
    emit errorOccurred("Invalid JSON from GitHub");
    return;
  }

  QJsonArray releases = doc.array();
  Version bestVersion = currentVersion;
  QString bestUrl;

  for (auto r : releases) {
    QJsonObject release = r.toObject();
    QString tag = release.value("tag_name").toString();
    bool prerelease = release.value("prerelease").toBool();
    Version releaseVer = Version::fromString(tag);

    // Beta/stable mantığı
    if (currentVersion.beta == 0) { // stable user
      if (prerelease)
        continue;
      if (currentVersion < releaseVer && bestVersion < releaseVer) {
        bestVersion = releaseVer;
        QJsonArray assets = release.value("assets").toArray();
        if (assets.empty()) return;
        for(auto a : assets) {
            QJsonObject asset = a.toObject();
            QString url = asset.value("browser_download_url").toString();
            if(url.endsWith(".exe")) {
                bestUrl = url;
                break;
            }
        }
      }
    } else { // beta user
      if (currentVersion < releaseVer && bestVersion < releaseVer) {
        bestVersion = releaseVer;
        QJsonArray assets = release.value("assets").toArray();
        if (!assets.isEmpty())
          bestUrl =
              assets[0].toObject().value("browser_download_url").toString();
      }
    }
  }

  if (bestVersion < currentVersion ||
      bestVersion.toString() == currentVersion.toString()) {
    m_updateAvailable = false;
    emit noUpdateFound();
    return;
  }

  m_latestVersion = bestVersion;
  m_latestUrl = bestUrl;
  m_updateAvailable = true;
  emit updateFound();
}
