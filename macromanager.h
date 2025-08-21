// macromanager.h - Enhanced Validation System Additions
#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <QVector>
#include <QString>
#include <QHash>
#include <QSet>
#include <optional>
#include <functional>

#include "Enums.h"

struct Macro {
    int id = -1;
    QString name;
    QString description;
    QString hotkey; // "DEF" | "F6" | "Shift + F6" ...
};

struct MacroAction {
    int macro_id = -1;
    int order = 0;
    ActionType action_type;   // "mouse" | "keyboard"
    ClickType click_type;    // "CLICK" | "HOLD" | "HOVER"
    int repeat = 1;        // 0 = infinite
    std::optional<QString> position;      // "x, y"
    bool current_pos = true;              // uses current cursor pos
    int interval = 0;                     // ms
    std::optional<int> hold_duration;     // ms
    std::optional<int> hover_duration;    // ms
    int click_count = 1;
    std::optional<MouseButton> mouse_button;  // "LEFT" | "RIGHT" | "MID" | ...
    QString key_name;
};

class MacroManager final : public QObject {
    Q_OBJECT
public:
    static MacroManager& instance(); // Meyers Singleton

    // init + schema
    bool init();
    bool isOpen() const;

    // Macros
    QVector<Macro> getAllMacros() const;
    bool existsMacro(int id);
    bool existsMacro(const QString& name);
    std::optional<Macro> getMacroById(int id) const;
    std::optional<Macro> getMacroByName(const QString& name) const;
    int  createMacro(const QString& name, const QString& description, const QString& hotkey, QString* error=nullptr);
    bool updateMacro(const Macro& m, QString* error=nullptr);
    bool deleteMacro(int id, QString* error=nullptr);

    // Actions
    QVector<MacroAction> getActions(int macroId) const;
    bool addAction(const MacroAction& a, QString* error=nullptr);
    bool updateAction(int macroId, int oldOrder, const MacroAction& newAction, QString* error=nullptr);
    bool deleteAction(int macroId, int order, QString* error=nullptr);
    bool setActionsForMacro(int macroId, const QVector<MacroAction>& actions, QString* error=nullptr); // transaction
    bool normalizeOrders(int macroId, QString* error=nullptr);

    bool moveActionUp(int macroId, int order, QString* error=nullptr);
    bool moveActionDown(int macroId, int order, QString* error=nullptr);
    bool swapActions(int macroId, int order1, int order2, QString* error=nullptr);

    // Basic Validation (existing)
    bool validateMacroName(const QString& name, QString* error=nullptr) const;
    bool validateMacroDescription(const QString& desc, QString* error=nullptr) const;
    bool validateHotkey(const QString& hotkey, QString* error=nullptr) const;
    bool validateAction(const MacroAction& a, QString* error=nullptr) const;

    // Path helper
    QString dbPath() const;

private:
    explicit MacroManager(QObject* parent=nullptr);
    MacroManager(const MacroManager&) = delete;
    MacroManager& operator=(const MacroManager&) = delete;

    bool ensureSchema(QString* error=nullptr);
    bool ensureDefaultMacro(QString* error=nullptr);
    bool execQuery(QSqlQuery& q, const char* ctx, QString* error) const;

    QSqlDatabase m_db;
};
