#ifndef VERSIONMANAGER_H
#define VERSIONMANAGER_H

#include <QObject>

/*
 * SÜRÜMLEME SİSTEMİ:
 * major.minor.patch-betaX
 *
 * (release'lerde beta olmaz)
 */
class VersionManager : public QObject {
    Q_OBJECT
public:
    static VersionManager& instance()
    {
        static VersionManager _instance;
        return _instance;
    }

    QString normalize(const QString& ver) {
        QString versionPart = ver;
        QString betaPart;

        // Beta kısmı varsa ayır
        int betaIndex = ver.indexOf("-beta");
        if (betaIndex != -1) {
            versionPart = ver.left(betaIndex);        // "2.0"
            betaPart = ver.mid(betaIndex);            // "-beta1"
        } else {
            betaPart = "";
        }

        QStringList numbers = versionPart.split(".");
        // Eksik olan parçaları tamamla
        while (numbers.size() < 3)
            numbers.append("0");

        QString normalizedVersion = numbers.join(".") + betaPart;
        return normalizedVersion;
    }


    void initVer(const QString& verstr) {
        m_versionString = normalize(verstr);
    }

    QString version() const {
        return m_versionString;
    }

    bool isRelease() const {
        return !m_versionString.contains("beta");
    }

    int major() const {
        return m_versionString.split("-")[0].split(".")[0].toInt();
    }

    int minor() const {
        return m_versionString.split("-")[0].split(".")[1].toInt();
    }

    int patch() const {
        return m_versionString.split("-")[0].split(".")[2].toInt();
    }

    int beta() const {
        return (m_versionString.contains("beta") ? m_versionString.split("-")[1].remove(0, 4) : "0").toInt();
    }

private:
    explicit VersionManager(QObject *parent = nullptr) : QObject(parent) {}

    VersionManager(const VersionManager&) = delete;
    VersionManager& operator=(const VersionManager&) = delete;

    ~VersionManager() override = default;

    QString m_versionString;
};


#endif // VERSIONMANAGER_H
