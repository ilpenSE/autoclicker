#include "Logger.h"
#include <QDateTime>
#include <QTextStream>
#include <QDebug>
#include <QDir>
#include "appdatamanager.h"

Logger& Logger::instance() {
    static Logger _instance;
    return _instance;
}

Logger::Logger(QObject* parent)
    : QObject(parent)
{
    // AppData/logs klasörü yolu
    QString logsDirPath = AppDataManager::instance().appFolderPath() + "/logs";

    // Klasör yoksa oluştur
    QDir dir;
    if (!dir.exists(logsDirPath)) {
        if (!dir.mkpath(logsDirPath)) {
            qWarning() << "[LOG/WARNING] Log klasörü oluşturulamadı:" << logsDirPath;
        }
    }

    // Dosya adı
    QString fileName = QDateTime::currentDateTime().toString("dd.MM.yyyy-hh.mm.ss") + ".log";
    QString filePath = logsDirPath + "/" + fileName;

    m_logFile.setFileName(filePath);

    if (!m_logFile.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "[LOG/WARNING] Log dosyası açılamadı:" << filePath;
    }
}

Logger::~Logger() {
    if (m_logFile.isOpen())
        m_logFile.close();
}

// theme logs
void Logger::thInfo(const QString& message) {
    log("[THEMES/INFO]", message);
}

void Logger::thWarning(const QString& message) {
    log("[THEMES/WARNING]", message);
}

void Logger::thError(const QString& message) {
    log("[THEMES/ERROR]", message);
}

// settings logs
void Logger::sInfo(const QString& message) {
    log("[SETTINGS/INFO]", message);
}

void Logger::sWarning(const QString& message) {
    log("[SETTINGS/WARNING]", message);
}

void Logger::sError(const QString& message) {
    log("[SETTINGS/ERROR]", message);
}

// file system logs
void Logger::fsInfo(const QString& message) {
    log("[FS/INFO]", message);
}

void Logger::fsWarning(const QString& message) {
    log("[FS/WARNING]", message);
}

void Logger::fsError(const QString& message) {
    log("[FS/ERROR]", message);
}

// language logs
void Logger::langInfo(const QString& message) {
    log("[LANG/INFO]", message);
}

void Logger::langWarning(const QString& message) {
    log("[LANG/WARNING]", message);
}

void Logger::langError(const QString& message) {
    log("[LANG/ERROR]", message);
}

// default logs
void Logger::logInfo(const QString& message) {
    log("[INFO]", message);
}

void Logger::logWarning(const QString &message) {
    log("[WARNING]", message);
}

void Logger::logError(const QString& message) {
    log("[ERROR]", message);
}

void Logger::log(const QString& level, const QString& message) {
    QString timeStamp = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss");
    QString logLine = QString("%1 %2 %3").arg(timeStamp, level, message);

    qDebug() << logLine;

    if (m_logFile.isOpen()) {
        QTextStream out(&m_logFile);
        out << logLine << "\n";
        out.flush();
    }
}
