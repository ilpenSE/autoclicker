#ifndef MacroManager_H
#define MacroManager_H

#include "logger.h"

#include <QObject>
#include <QDir>
#include <QStandardPaths>
#include <QFile>
#include <QDebug>
#include <QJsonDocument>

class MacroManager : public QObject {
    Q_OBJECT
public:
    static MacroManager& instance()
    {
        static MacroManager _instance;
        return _instance;
    }

    // fonksiyonlar

private:
    explicit MacroManager(QObject *parent = nullptr) : QObject(parent) {}

    // Copy ve assign engelle
    MacroManager(const MacroManager&) = delete;
    MacroManager& operator=(const MacroManager&) = delete;

    ~MacroManager() override = default;
};

#endif // MACROMANAGER_H
