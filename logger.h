#ifndef LOGGER_H
#define LOGGER_H

#include <QObject>
#include <QFile>

class Logger : public QObject
{
    Q_OBJECT
public:
    static Logger& instance();

    // default log
    void logInfo(const QString& message);
    void logWarning(const QString& message);
    void logError(const QString& message);

    // fs log
    void fsInfo(const QString& message);
    void fsWarning(const QString& message);
    void fsError(const QString& message);

    // lang log
    void langInfo(const QString& message);
    void langWarning(const QString& message);
    void langError(const QString& message);

    // settings log
    void sInfo(const QString& message);
    void sWarning(const QString& message);
    void sError(const QString& message);

    // themes log
    void thInfo(const QString& message);
    void thWarning(const QString& message);
    void thError(const QString& message);

    // macros log
    void mInfo(const QString& message);
    void mWarning(const QString& message);
    void mError(const QString& message);

private:
    explicit Logger(QObject* parent = nullptr);
    ~Logger() override;

    void log(const QString& level, const QString& message);

    QFile m_logFile;

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};

#endif // LOGGER_H
