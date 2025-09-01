#include "versioninfo.h"
#include <QDebug>
#include <QRegularExpression>

Version Version::fromString(const QString &str) {
  QRegularExpression re(R"((\d+)\.(\d+)(?:\.(\d+))?(?:-beta(\d+))?)");
  QRegularExpressionMatch match = re.match(str);
  Version v{0, 0, 0, 0};
  if (match.hasMatch()) {
    v.major = match.captured(1).toInt();
    v.minor = match.captured(2).toInt();
    v.patch = match.captured(3).isEmpty() ? 0 : match.captured(3).toInt();
    v.beta = match.captured(4).isEmpty() ? 0 : match.captured(4).toInt();
  }
  return v;
}

bool Version::operator<(const Version &other) const {
  if (major != other.major)
    return major < other.major;
  if (minor != other.minor)
    return minor < other.minor;
  if (patch != other.patch)
    return patch < other.patch;
  return beta < other.beta; // beta0 < beta1 < stable
}

QString Version::toString() const {
  if (beta == 0)
    return QString("%1.%2.%3").arg(major).arg(minor).arg(patch);
  return QString("%1.%2.%3-beta%4").arg(major).arg(minor).arg(patch).arg(beta);
}

bool VersionInfo::loadFromSettings(const QString &settingsPath) {
  QFile file(settingsPath);
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    qWarning() << "Cannot open settings.json:" << settingsPath;
    return false;
  }
  QByteArray data = file.readAll();
  file.close();

  QJsonParseError parseError;
  QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
  if (parseError.error != QJsonParseError::NoError || !doc.isObject()) {
    qWarning() << "JSON parse error:" << parseError.errorString();
    return false;
  }

  QJsonObject obj = doc.object();
  QString versionStr = obj.value("Version").toString();
  if (versionStr.isEmpty()) {
    qWarning() << "Version field missing in settings.json";
    return false;
  }

  m_version = Version::fromString(versionStr);
  return true;
}
