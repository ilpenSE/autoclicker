#ifndef LOGGERSTREAM_H
#define LOGGERSTREAM_H
#include <sstream>
#include "logger.h"
class LoggerStream {
public:
    LoggerStream(void (Logger::*logFunc)(const QString&))
        : m_logFunc(logFunc) {}

    ~LoggerStream() {
        QString msg = QString::fromStdString(m_stream.str());
        (Logger::instance().*m_logFunc)(msg);
    }

    template<typename T>
    LoggerStream& operator<<(const T& value) {
        m_stream << value;
        return *this;
    }

private:
    std::ostringstream m_stream;
    void (Logger::*m_logFunc)(const QString&);
};

#endif // LOGGERSTREAM_H
