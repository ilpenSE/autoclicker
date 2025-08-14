#include "macromanager.h"
#include "logger.h"
#include "appdatamanager.h"

#include <QStandardPaths>
#include <QDir>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QVariant>
#include <QRegularExpression>

static const int DEFAULT_MACRO_ID = 1;

MacroManager& MacroManager::instance() {
    static MacroManager inst;
    return inst;
}

MacroManager::MacroManager(QObject* parent) : QObject(parent) {}

QString MacroManager::dbPath() const {
    const auto base = AppDataManager::instance().appFolderPath();
    // const auto base = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    // AppDataLocation -> %APPDATA%/AutoClicker2 (Windows), ~/.local/share/AutoClicker2 (Linux)
    QDir().mkpath(base);
    return QDir(base).filePath("macros.sqlite");
}

bool MacroManager::init() {
    if (m_db.isValid() && m_db.isOpen()) return true;

    const QString path = dbPath();
    m_db = QSqlDatabase::addDatabase("QSQLITE", "macros_conn");
    m_db.setDatabaseName(path);
    if (!m_db.open()) return false;

    QString err;
    if (!ensureSchema(&err)) { Logger::instance().mWarning("ensureSchema failed:" + err); return false; }
    if (!ensureDefaultMacro(&err)) { Logger::instance().mError("ensureDefaultMacro failed:" + err); return false; }
    return true;
}

bool MacroManager::isOpen() const { return m_db.isOpen(); }

bool MacroManager::ensureSchema(QString* error) {
    QSqlQuery q(m_db);

    // Enable foreign keys
    if (!q.exec("PRAGMA foreign_keys = ON;")) return execQuery(q, "PRAGMA", error);

    // Macros
    const char* createMacros =
        "CREATE TABLE IF NOT EXISTS Macros ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " name TEXT NOT NULL UNIQUE,"
        " description TEXT NOT NULL,"
        " hotkey TEXT NOT NULL"
        ");";
    if (!q.exec(createMacros)) return execQuery(q, "create Macros", error);

    // MacroActions
    const char* createActions =
        "CREATE TABLE IF NOT EXISTS MacroActions ("
        " id INTEGER PRIMARY KEY AUTOINCREMENT,"
        " macro_id INTEGER NOT NULL,"
        " \"order\" INTEGER NOT NULL,"
        " action_type TEXT NOT NULL,"
        " click_type TEXT NOT NULL,"
        " repeat INTEGER NOT NULL,"
        " position TEXT NULL,"
        " current_pos INTEGER NOT NULL,"   // 0/1
        " interval INTEGER NOT NULL,"
        " hold_duration INTEGER NULL,"
        " hover_duration INTEGER NULL,"
        " click_count INTEGER NOT NULL,"
        " mouse_button TEXT NULL,"
        " FOREIGN KEY(macro_id) REFERENCES Macros(id) ON DELETE CASCADE"
        ");";
    if (!q.exec(createActions)) return execQuery(q, "create MacroActions", error);

    return true;
}

bool MacroManager::ensureDefaultMacro(QString* error) {
    QSqlQuery q(m_db);
    if (!q.exec("SELECT COUNT(*) FROM Macros;")) return execQuery(q, "count Macros", error);
    int cnt = 0;
    if (q.next()) cnt = q.value(0).toInt();
    if (cnt > 0) return true;

    // Insert DEFAULT macro and single default action inside a transaction
    if (!m_db.transaction()) return false;

    QSqlQuery insM(m_db);
    insM.prepare("INSERT INTO Macros(id, name, description, hotkey) VALUES(?, ?, ?, ?)");
    insM.addBindValue(DEFAULT_MACRO_ID);
    insM.addBindValue("DEFAULT");
    insM.addBindValue("The defaults.");
    insM.addBindValue("F6");
    if (!insM.exec()) { m_db.rollback(); return execQuery(insM, "insert DEFAULT Macro", error); }

    QSqlQuery insA(m_db);
    insA.prepare("INSERT INTO MacroActions("
                 "macro_id, \"order\", action_type, click_type, repeat, position, current_pos,"
                 " interval, hold_duration, hover_duration, click_count, mouse_button)"
                 " VALUES(?,?,?,?,?,?,?,?,?,?,?,?)");
    insA.addBindValue(DEFAULT_MACRO_ID);
    insA.addBindValue(0);
    insA.addBindValue("mouse");
    insA.addBindValue("CLICK");
    insA.addBindValue(0);
    insA.addBindValue("0, 0");
    insA.addBindValue(1);
    insA.addBindValue(100);
    insA.addBindValue(1000);
    insA.addBindValue(1000);
    insA.addBindValue(1);
    insA.addBindValue("LEFT");
    if (!insA.exec()) { m_db.rollback(); return execQuery(insA, "insert DEFAULT Action", error); }

    if (!m_db.commit()) { m_db.rollback(); return false; }
    return true;
}

bool MacroManager::execQuery(QSqlQuery& q, const char* ctx, QString* error) const {
    if (error) *error = QString("%1: %2").arg(ctx).arg(q.lastError().text());
    return false;
}

// ===== Validation =====
bool MacroManager::validateMacroName(const QString& name, QString* error) const {
    // 1-20 chars, letters/digits/_/-/space
    static QRegularExpression rx(R"(^[A-Za-zöçşğüÖÇŞİĞ0-9 _-]{1,20}$)");
    if (!rx.match(name).hasMatch()) {
        if (error) *error = "Name length can be between 1-20 and only contains A-Z a-z 0-9 _ - and space.";
        return false;
    }

    if (getMacroByName(name)) {
        if (error) *error = "A macro with this name exists!";
        return false;
    }

    return true;
}
bool MacroManager::validateMacroDescription(const QString& desc, QString* error) const {
    static QRegularExpression rx(R"(^[A-Za-zöçşğüÖÇŞİĞ0-9 _-]{1,100}$)");
    if (!rx.match(desc).hasMatch()) {
        if (error) *error = "Description can be between 1-100 and only contains A-Z a-z 0-9 _ - and space.";
        return false;
    }
    return true;
}
bool MacroManager::validateHotkey(const QString& hotkey, QString* error) const {
    // "DEF" or "[mod + ]KEY"  (mods: Ctrl|Shift|Alt), disallow LWin|RWin|Fn|Enter
    if (hotkey == "DEF") return true;

    // Examples allowed: "F7", "Shift + F6", "Ctrl + Alt + A"
    static QRegularExpression rx(
        R"(^(?:(?:Ctrl|Shift|Alt)\s*\+\s*){0,3}(?:F([1-9]|1[0-9]|2[0-4])|[A-Z0-9])$)"
        );
    if (!rx.match(hotkey).hasMatch()) {
        if (error) *error = "Invalid hotkey. Eg: DEF, F7, Shift + F6, Ctrl + Alt + A";
        return false;
    }
    // guard forbidden keys
    if (hotkey.contains("LWin", Qt::CaseInsensitive) ||
        hotkey.contains("RWin", Qt::CaseInsensitive) ||
        hotkey.contains("Fn",   Qt::CaseInsensitive) ||
        hotkey.compare("Enter", Qt::CaseInsensitive) == 0) {
        if (error) *error = "Hotkey can't contain LWin, RWin, Fn, Enter.";
        return false;
    }
    return true;
}

template<typename T, typename... Args>
bool equalsAtLeastOne(const T& query, const Args&... values) {
    return ((query == values) || ...);  // C++17 fold expression
}

bool MacroManager::validateAction(const MacroAction& a, QString* error) const {
    if (a.macro_id <= 0) { if (error) *error = "invalid macro_id."; return false; }
    if (!equalsAtLeastOne(a.action_type, ActionType::KEYBOARD, ActionType::MOUSE)) { if (error) *error = "action_type must be 'mouse' or 'keyboard'."; return false; }
    if (!equalsAtLeastOne(a.click_type, ClickType::CLICK, ClickType::HOLD, ClickType::HOVER)) { if (error) *error = "click_type must be CLICK/HOLD/HOVER."; return false; }
    if (a.repeat < 0) { if (error) *error = "Repeat can't be negative (0 = infinite)."; return false; }
    if (a.interval < 0) { if (error) *error = "interval can't be negative."; return false; }
    if (a.click_count < 1) { if (error) *error = "click_count must be at least 1."; return false; }

    return true;
}

// ===== Macros CRUD =====
QVector<Macro> MacroManager::getAllMacros() const {
    QVector<Macro> out;
    QSqlQuery q(m_db);
    q.exec("SELECT id, name, description, hotkey FROM Macros ORDER BY id ASC;");
    while (q.next()) {
        Macro m;
        m.id = q.value(0).toInt();
        m.name = q.value(1).toString();
        m.description = q.value(2).toString();
        m.hotkey = q.value(3).toString();
        out.push_back(std::move(m));
    }
    return out;
}

std::optional<Macro> MacroManager::getMacroById(int id) const {
    QSqlQuery q(m_db);
    q.prepare("SELECT id, name, description, hotkey FROM Macros WHERE id=?");
    q.addBindValue(id);
    if (!q.exec()) return std::nullopt;
    if (!q.next()) return std::nullopt;
    Macro m;
    m.id = q.value(0).toInt();
    m.name = q.value(1).toString();
    m.description = q.value(2).toString();
    m.hotkey = q.value(3).toString();
    return m;
}
std::optional<Macro> MacroManager::getMacroByName(const QString& name) const {
    QSqlQuery q(m_db);
    q.prepare("SELECT id, name, description, hotkey FROM Macros WHERE name=?");
    q.addBindValue(name);
    if (!q.exec() || !q.next()) return std::nullopt;
    Macro m;
    m.id = q.value(0).toInt(); m.name = q.value(1).toString(); m.description = q.value(2).toString(); m.hotkey = q.value(3).toString();
    return m;
}

int MacroManager::createMacro(const QString& name, const QString& description, const QString& hotkey, QString* error) {
    if (!validateMacroName(name, error) || !validateMacroDescription(description, error) || !validateHotkey(hotkey, error))
        return -1;

    QSqlQuery q(m_db);
    q.prepare("INSERT INTO Macros(name, description, hotkey) VALUES(?,?,?)");
    q.addBindValue(name);
    q.addBindValue(description);
    q.addBindValue(hotkey);
    if (!q.exec()) { execQuery(q, "insert Macro", error); return -1; }
    return q.lastInsertId().toInt();
}

bool MacroManager::updateMacro(const Macro& m, QString* error) {
    if (m.id == DEFAULT_MACRO_ID) { if (error) *error="DEFAULT makrosu düzenlenemez."; return false; }
    if (!validateMacroName(m.name, error) || !validateMacroDescription(m.description, error) || !validateHotkey(m.hotkey, error))
        return false;

    QSqlQuery q(m_db);
    q.prepare("UPDATE Macros SET name=?, description=?, hotkey=? WHERE id=?");
    q.addBindValue(m.name);
    q.addBindValue(m.description);
    q.addBindValue(m.hotkey);
    q.addBindValue(m.id);
    return q.exec();
}

bool MacroManager::deleteMacro(int id, QString* error) {
    if (id == DEFAULT_MACRO_ID) { if (error) *error="DEFAULT makrosu silinemez."; return false; }
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM Macros WHERE id=?");
    q.addBindValue(id);
    if (!q.exec()) return execQuery(q, "delete Macro", error);
    return true;
}

// ===== Actions CRUD =====
QVector<MacroAction> MacroManager::getActions(int macroId) const {
    QVector<MacroAction> out;
    QSqlQuery q(m_db);
    q.prepare("SELECT id, macro_id, \"order\", action_type, click_type, repeat, position, current_pos,"
              " interval, hold_duration, hover_duration, click_count, mouse_button "
              "FROM MacroActions WHERE macro_id=? ORDER BY \"order\" ASC, id ASC");
    q.addBindValue(macroId);
    if (!q.exec()) return out;
    while (q.next()) {
        MacroAction a;
        a.id = q.value(0).toInt();
        a.macro_id = q.value(1).toInt();
        a.order = q.value(2).toInt();
        a.repeat = q.value(5).toInt();
        a.position = q.isNull(6) ? std::optional<QString>{} : std::optional<QString>{q.value(6).toString()};
        a.current_pos = q.value(7).toInt() != 0;
        a.interval = q.value(8).toInt();
        a.hold_duration = q.isNull(9) ? std::optional<int>{} : std::optional<int>{q.value(9).toInt()};
        a.hover_duration = q.isNull(10) ? std::optional<int>{} : std::optional<int>{q.value(10).toInt()};
        a.click_count = q.value(11).toInt();
        out.push_back(std::move(a));

        // action type enum cast
        QString strAct = q.value(3).toString();
        auto actOpt = strToActionType(strAct);
        if (!actOpt) {
            Logger::instance().mError("Invalid action type: " + strAct + " in " + QString::number(a.id));
        }
        a.action_type = *actOpt;

        // click type enum cast
        QString strClick = q.value(4).toString();
        auto clickOpt = strToClickType(strClick);
        if (!clickOpt) {
            Logger::instance().mError("Invalid click type: " + strClick + " in " + QString::number(a.id));
        }
        a.click_type = *clickOpt;

        // mouse button enum cast
        if (!q.isNull(12)) {
            auto btnOpt = strToMouseButton(q.value(12).toString());
            if (btnOpt) a.mouse_button = *btnOpt;
        }
    }
    return out;
}

int MacroManager::addAction(const MacroAction& a, QString* error) {
    if (!validateAction(a, error)) return -1;

    // Determine next order if caller didn't set meaningful order
    int ord = a.order;
    if (ord < 0) {
        QSqlQuery q(m_db);
        q.prepare("SELECT IFNULL(MAX(\"order\"), -1) + 1 FROM MacroActions WHERE macro_id=?");
        q.addBindValue(a.macro_id);
        if (!q.exec() || !q.next()) { execQuery(q, "max order", error); return -1; }
        ord = q.value(0).toInt();
    }

    QSqlQuery q(m_db);
    q.prepare("INSERT INTO MacroActions(macro_id, \"order\", action_type, click_type, repeat, position, current_pos,"
              " interval, hold_duration, hover_duration, click_count, mouse_button)"
              " VALUES(?,?,?,?,?,?,?,?,?,?,?,?)");
    q.addBindValue(a.macro_id);
    q.addBindValue(ord);
    q.addBindValue(actionTypeToStr(a.action_type));
    q.addBindValue(clickTypeToStr(a.click_type));
    q.addBindValue(a.repeat);
    if (a.position.has_value()) q.addBindValue(a.position.value()); else q.addBindValue(QVariant(QVariant::String));
    q.addBindValue(a.current_pos ? 1 : 0);
    q.addBindValue(a.interval);
    if (a.hold_duration.has_value()) q.addBindValue(a.hold_duration.value()); else q.addBindValue(QVariant(QVariant::Int));
    if (a.hover_duration.has_value()) q.addBindValue(a.hover_duration.value()); else q.addBindValue(QVariant(QVariant::Int));
    q.addBindValue(a.click_count);
    if (a.mouse_button.has_value()) q.addBindValue(mouseButtonToStr(a.mouse_button.value())); else q.addBindValue(QVariant(QVariant::String));
    if (!q.exec()) { execQuery(q, "insert Action", error); return -1; }
    return q.lastInsertId().toInt();
}

bool MacroManager::updateAction(const MacroAction& a, QString* error) {
    if (!validateAction(a, error)) return false;

    QSqlQuery q(m_db);
    q.prepare("UPDATE MacroActions SET macro_id=?, \"order\"=?, action_type=?, click_type=?, repeat=?, position=?,"
              " current_pos=?, interval=?, hold_duration=?, hover_duration=?, click_count=?, mouse_button=? WHERE id=?");
    q.addBindValue(a.macro_id);
    q.addBindValue(a.order);
    q.addBindValue(actionTypeToStr(a.action_type));
    q.addBindValue(clickTypeToStr(a.click_type));
    q.addBindValue(a.repeat);
    if (a.position.has_value()) q.addBindValue(a.position.value()); else q.addBindValue(QVariant(QVariant::String));
    q.addBindValue(a.current_pos ? 1 : 0);
    q.addBindValue(a.interval);
    if (a.hold_duration.has_value()) q.addBindValue(a.hold_duration.value()); else q.addBindValue(QVariant(QVariant::Int));
    if (a.hover_duration.has_value()) q.addBindValue(a.hover_duration.value()); else q.addBindValue(QVariant(QVariant::Int));
    q.addBindValue(a.click_count);
    if (a.mouse_button.has_value()) q.addBindValue(mouseButtonToStr(a.mouse_button.value())); else q.addBindValue(QVariant(QVariant::String));
    q.addBindValue(a.id);
    if (!q.exec()) return execQuery(q, "update Action", error);
    return true;
}

bool MacroManager::deleteAction(int actionId, QString* error) {
    QSqlQuery q(m_db);
    q.prepare("DELETE FROM MacroActions WHERE id=?");
    q.addBindValue(actionId);
    if (!q.exec()) return execQuery(q, "delete Action", error);
    return true;
}

bool MacroManager::setActionsForMacro(int macroId, const QVector<MacroAction>& actions, QString* error) {
    if (!m_db.transaction()) { if (error) *error="transaction begin failed"; return false; }

    QSqlQuery del(m_db);
    del.prepare("DELETE FROM MacroActions WHERE macro_id=?");
    del.addBindValue(macroId);
    if (!del.exec()) { m_db.rollback(); return execQuery(del, "clear actions", error); }

    QSqlQuery ins(m_db);
    ins.prepare("INSERT INTO MacroActions(macro_id, \"order\", action_type, click_type, repeat, position, current_pos,"
                " interval, hold_duration, hover_duration, click_count, mouse_button)"
                " VALUES(?,?,?,?,?,?,?,?,?,?,?,?)");

    int order = 0;
    for (const auto& aIn : actions) {
        MacroAction a = aIn;
        a.macro_id = macroId;
        a.order = order++;
        if (!validateAction(a, error)) { m_db.rollback(); return false; }

        ins.bindValue(0, a.macro_id);
        ins.bindValue(1, a.order);
        ins.bindValue(2, actionTypeToStr(a.action_type));
        ins.bindValue(3, clickTypeToStr(a.click_type));
        ins.bindValue(4, a.repeat);
        ins.bindValue(5, a.position.has_value() ? QVariant(a.position.value()) : QVariant(QVariant::String));
        ins.bindValue(6, a.current_pos ? 1 : 0);
        ins.bindValue(7, a.interval);
        ins.bindValue(8, a.hold_duration.has_value() ? QVariant(a.hold_duration.value()) : QVariant(QVariant::Int));
        ins.bindValue(9, a.hover_duration.has_value() ? QVariant(a.hover_duration.value()) : QVariant(QVariant::Int));
        ins.bindValue(10, a.click_count);
        ins.bindValue(11, a.mouse_button.has_value() ? QVariant(mouseButtonToStr(a.mouse_button.value())) : QVariant(QVariant::String));

        if (!ins.exec()) { m_db.rollback(); return execQuery(ins, "insert batch action", error); }
    }

    if (!m_db.commit()) { m_db.rollback(); if (error) *error="commit failed"; return false; }
    return true;
}

bool MacroManager::normalizeOrders(int macroId, QString* error) {
    auto acts = getActions(macroId);
    if (acts.isEmpty()) return true;
    for (int i=0;i<acts.size();++i) acts[i].order = i;
    return setActionsForMacro(macroId, acts, error);
}
