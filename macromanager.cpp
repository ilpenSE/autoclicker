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

    // Migration kontrolü - eğer tablo AUTOINCREMENT ile varsa migrate et
    QSqlQuery checkQ(m_db);
    if (!checkQ.exec("SELECT name FROM sqlite_master WHERE type='table' AND name='Macros'")) {
        return execQuery(checkQ, "check table exists", error);
    }

    bool tableExists = checkQ.next();
    checkQ.finish(); // Query'yi kapat

    if (tableExists) {
        // Tablo mevcut, AUTOINCREMENT kontrolü yap
        QSqlQuery schemaQ(m_db);
        if (!schemaQ.exec("SELECT sql FROM sqlite_master WHERE type='table' AND name='Macros'")) {
            return execQuery(schemaQ, "check table schema", error);
        }
        if (schemaQ.next()) {
            QString createSql = schemaQ.value(0).toString();
            schemaQ.finish(); // Query'yi kapat

            if (createSql.contains("AUTOINCREMENT", Qt::CaseInsensitive)) {
                Logger::instance().mInfo("Migrating table from AUTOINCREMENT to sequential IDs...");
                if (!migrateToSequentialIds(error)) {
                    return false;
                }
                Logger::instance().mInfo("Migration completed successfully");
            }
        }
    } else {
        // Tablo yok, yeni oluştur
        const char* createMacros =
            "CREATE TABLE Macros ("
            " id INTEGER PRIMARY KEY,"  // AUTOINCREMENT yok
            " name TEXT NOT NULL UNIQUE,"
            " description TEXT NOT NULL,"
            " hotkey TEXT NOT NULL"
            ");";
        if (!q.exec(createMacros)) return execQuery(q, "create Macros", error);
    }

    const char* createActions =
        "CREATE TABLE IF NOT EXISTS MacroActions ("
        " macro_id INTEGER NOT NULL,"
        " \"order\" INTEGER NOT NULL,"
        " action_type TEXT NOT NULL,"
        " click_type TEXT NOT NULL,"
        " repeat INTEGER NOT NULL,"
        " position TEXT NULL,"
        " current_position INTEGER NOT NULL,"
        " interval INTEGER NOT NULL,"
        " hold_duration INTEGER NULL,"
        " hover_duration INTEGER NULL,"
        " click_count INTEGER NOT NULL,"
        " mouse_button TEXT NULL,"
        " PRIMARY KEY(macro_id, \"order\"),"
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
    insM.addBindValue("DEF");
    if (!insM.exec()) { m_db.rollback(); return execQuery(insM, "insert DEFAULT Macro", error); }

    QSqlQuery insA(m_db);
    insA.prepare("INSERT INTO MacroActions("
                 "macro_id, \"order\", action_type, click_type, repeat, position, current_position,"
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

// ===== Basic Validation =====
bool MacroManager::validateMacroName(const QString& name, QString* error) const {
    // 1-20 chars, letters/digits/_/-/space
    static QRegularExpression rx(R"(^[A-Za-zöçşğüÖÇŞİĞ0-9 _-]{1,20}$)");
    if (!rx.match(name).hasMatch()) {
        if (error) *error = "Name length can be between 1-20 and only contains A-Z a-z 0-9 _ - and space: " + name;
        return false;
    }

    return true;
}
bool MacroManager::validateMacroDescription(const QString& desc, QString* error) const {
    static QRegularExpression rx(R"(^[A-Za-zöçşğüÖÇŞİĞ0-9 _-]{1,100}$)");
    if (!rx.match(desc).hasMatch()) {
        if (error) *error = "Description can be between 1-100 and only contains A-Z a-z 0-9 _ - and space: " + desc;
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
        if (error) *error = "Invalid hotkey: " + hotkey;
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

bool MacroManager::existsMacro(int id) {
    return getMacroById(id) != std::nullopt;
}

bool MacroManager::existsMacro(const QString& name) {
    return getMacroByName(name) != std::nullopt;
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

    if (existsMacro(name)) {
        if (error) *error = "A macro with this name exists!";
        return -1;
    }

    QSqlQuery q(m_db);
    q.prepare("INSERT INTO Macros(name, description, hotkey) VALUES(?,?,?)");
    q.addBindValue(name);
    q.addBindValue(description);
    q.addBindValue(hotkey);
    if (!q.exec()) {
        execQuery(q, "insert Macro", error);
        return -1;
    }
    return q.lastInsertId().toInt();
}

bool MacroManager::updateMacro(const Macro& m, QString* error) {
    if (m.id == DEFAULT_MACRO_ID) { if (error) *error="DEFAULT cannot be edited."; return false; }
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

// db: def, m1, m2
// ui: def, m1
bool MacroManager::updateAllMacros(QVector<Macro> macros) {
    return true;
}

bool MacroManager::deleteMacro(int id, QString* error) {
    if (id == DEFAULT_MACRO_ID) { if (error) *error="DEFAULT cannot be deleted."; return false; }
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
    q.prepare("SELECT macro_id, \"order\", action_type, click_type, repeat, position, current_position,"
              " interval, hold_duration, hover_duration, click_count, mouse_button "
              "FROM MacroActions WHERE macro_id=? ORDER BY \"order\" ASC");
    q.addBindValue(macroId);
    if (!q.exec()) return out;

    while (q.next()) {
        MacroAction a;
        a.macro_id = q.value(0).toInt();
        a.order = q.value(1).toInt();
        a.repeat = q.value(4).toInt();
        a.position = q.isNull(5) ? std::optional<QString>{} : std::optional<QString>{q.value(5).toString()};
        a.current_pos = q.value(6).toInt() != 0;
        a.interval = q.value(7).toInt();
        a.hold_duration = q.isNull(8) ? std::optional<int>{} : std::optional<int>{q.value(8).toInt()};
        a.hover_duration = q.isNull(9) ? std::optional<int>{} : std::optional<int>{q.value(9).toInt()};
        a.click_count = q.value(10).toInt();

        // Enum casting'ler
        QString strAct = q.value(2).toString();
        auto actOpt = strToActionType(strAct);
        if (!actOpt) {
            Logger::instance().mError("Invalid action type: " + strAct + " for macro " + QString::number(a.macro_id) + " order " + QString::number(a.order));
            continue;
        }
        a.action_type = *actOpt;

        QString strClick = q.value(3).toString();
        auto clickOpt = strToClickType(strClick);
        if (!clickOpt) {
            Logger::instance().mError("Invalid click type: " + strClick + " for macro " + QString::number(a.macro_id) + " order " + QString::number(a.order));
            continue;
        }
        a.click_type = *clickOpt;

        if (!q.isNull(11)) {
            auto btnOpt = strToMouseButton(q.value(11).toString());
            if (btnOpt) {
                a.mouse_button = *btnOpt;
            }
        }

        out.push_back(std::move(a));
    }
    return out;
}

bool MacroManager::addAction(const MacroAction& a, QString* error) {
    if (!validateAction(a, error)) return false;

    Logger::instance().mInfo(QString("Adding action: macro_id=%1, action_type=%2")
                                 .arg(a.macro_id).arg(actionTypeToStr(a.action_type)));

    if (!m_db.transaction()) {
        if (error) *error = "Transaction failed";
        return false;
    }

    // Her zaman en sona ekle - order'ı otomatik belirle
    QSqlQuery maxQ(m_db);
    maxQ.prepare("SELECT IFNULL(MAX(\"order\"), -1) + 1 FROM MacroActions WHERE macro_id=?");
    maxQ.addBindValue(a.macro_id);
    if (!maxQ.exec() || !maxQ.next()) {
        m_db.rollback();
        execQuery(maxQ, "get max order", error);
        return false;
    }
    int targetOrder = maxQ.value(0).toInt();

    Logger::instance().mInfo(QString("Auto-assigned order: %1").arg(targetOrder));

    // Yeni action'ı ekle - targetOrder kullan, a.order DEĞİL!
    QSqlQuery insQ(m_db);
    insQ.prepare("INSERT INTO MacroActions(macro_id, \"order\", action_type, click_type, repeat, position, current_position,"
                 " interval, hold_duration, hover_duration, click_count, mouse_button)"
                 " VALUES(?,?,?,?,?,?,?,?,?,?,?,?)");
    insQ.addBindValue(a.macro_id);
    insQ.addBindValue(targetOrder);  // Burada targetOrder kullanıyoruz
    insQ.addBindValue(actionTypeToStr(a.action_type));
    insQ.addBindValue(clickTypeToStr(a.click_type));
    insQ.addBindValue(a.repeat);
    if (a.position.has_value()) insQ.addBindValue(a.position.value()); else insQ.addBindValue(QVariant(QVariant::String));
    insQ.addBindValue(a.current_pos ? 1 : 0);
    insQ.addBindValue(a.interval);
    if (a.hold_duration.has_value()) insQ.addBindValue(a.hold_duration.value()); else insQ.addBindValue(QVariant(QVariant::Int));
    if (a.hover_duration.has_value()) insQ.addBindValue(a.hover_duration.value()); else insQ.addBindValue(QVariant(QVariant::Int));
    insQ.addBindValue(a.click_count);
    if (a.mouse_button.has_value()) insQ.addBindValue(mouseButtonToStr(a.mouse_button.value())); else insQ.addBindValue(QVariant(QVariant::String));

    if (!insQ.exec()) {
        m_db.rollback();
        execQuery(insQ, "insert Action", error);
        return false;
    }

    if (!m_db.commit()) {
        m_db.rollback();
        if (error) *error = "Commit failed";
        return false;
    }

    Logger::instance().mInfo(QString("Action added successfully with order: %1").arg(targetOrder));
    return true;
}

// deleteAction() da order'ları normalize etsin:
bool MacroManager::deleteAction(int macroId, int order, QString* error) {
    if (!m_db.transaction()) {
        if (error) *error = "Transaction failed";
        return false;
    }

    // Action'ı sil
    QSqlQuery delQ(m_db);
    delQ.prepare("DELETE FROM MacroActions WHERE macro_id=? AND \"order\"=?");
    delQ.addBindValue(macroId);
    delQ.addBindValue(order);
    if (!delQ.exec()) {
        m_db.rollback();
        return execQuery(delQ, "delete Action", error);
    }

    // Silinen action'dan sonraki order'ları 1 azalt
    QSqlQuery shiftQ(m_db);
    shiftQ.prepare("UPDATE MacroActions SET \"order\" = \"order\" - 1 "
                   "WHERE macro_id=? AND \"order\" > ?");
    shiftQ.addBindValue(macroId);
    shiftQ.addBindValue(order);
    if (!shiftQ.exec()) {
        m_db.rollback();
        return execQuery(shiftQ, "shift orders after delete", error);
    }

    if (!m_db.commit()) {
        m_db.rollback();
        if (error) *error = "Commit failed";
        return false;
    }

    return true;
}

// Bir action'ı yukarı taşı
bool MacroManager::moveActionUp(int macroId, int order, QString* error) {
    if (order <= 0) {
        if (error) *error = "Already at top position";
        return false;
    }
    return swapActions(macroId, order, order - 1, error);
}

// Bir action'ı aşağı taşı
bool MacroManager::moveActionDown(int macroId, int order, QString* error) {
    // Maksimum order'ı kontrol et
    QSqlQuery maxQ(m_db);
    maxQ.prepare("SELECT MAX(\"order\") FROM MacroActions WHERE macro_id=?");
    maxQ.addBindValue(macroId);
    if (!maxQ.exec() || !maxQ.next()) {
        return execQuery(maxQ, "get max order", error);
    }
    int maxOrder = maxQ.value(0).toInt();

    if (order >= maxOrder) {
        if (error) *error = "Already at bottom position";
        return false;
    }
    return swapActions(macroId, order, order + 1, error);
}

// İki action'ın yerlerini değiştir
bool MacroManager::swapActions(int macroId, int order1, int order2, QString* error) {
    if (!m_db.transaction()) {
        if (error) *error = "Transaction failed";
        return false;
    }

    // Geçici order kullanarak swap yap (-1000 gibi)
    const int tempOrder = -1000;

    QSqlQuery q(m_db);

    // İlk action'ı geçici order'a taşı
    q.prepare("UPDATE MacroActions SET \"order\"=? WHERE macro_id=? AND \"order\"=?");
    q.addBindValue(tempOrder);
    q.addBindValue(macroId);
    q.addBindValue(order1);
    if (!q.exec()) {
        m_db.rollback();
        return execQuery(q, "move to temp", error);
    }

    // İkinci action'ı birincinin yerine taşı
    q.prepare("UPDATE MacroActions SET \"order\"=? WHERE macro_id=? AND \"order\"=?");
    q.addBindValue(order1);
    q.addBindValue(macroId);
    q.addBindValue(order2);
    if (!q.exec()) {
        m_db.rollback();
        return execQuery(q, "move second", error);
    }

    // Geçici action'ı ikincinin yerine taşı
    q.prepare("UPDATE MacroActions SET \"order\"=? WHERE macro_id=? AND \"order\"=?");
    q.addBindValue(order2);
    q.addBindValue(macroId);
    q.addBindValue(tempOrder);
    if (!q.exec()) {
        m_db.rollback();
        return execQuery(q, "move from temp", error);
    }

    if (!m_db.commit()) {
        m_db.rollback();
        if (error) *error = "Commit failed";
        return false;
    }

    return true;
}

bool MacroManager::updateAction(int macroId, int oldOrder, const MacroAction& newAction, QString* error) {
    if (!validateAction(newAction, error)) return false;

    if (!m_db.transaction()) { if (error) *error = "Transaction failed"; return false; }

    // Eski action'ı sil
    QSqlQuery delQ(m_db);
    delQ.prepare("DELETE FROM MacroActions WHERE macro_id=? AND \"order\"=?");
    delQ.addBindValue(macroId);
    delQ.addBindValue(oldOrder);
    if (!delQ.exec()) {
        m_db.rollback();
        execQuery(delQ, "delete old action", error);
        return false;
    }

    // Yeni action'ı ekle
    QSqlQuery insQ(m_db);
    insQ.prepare("INSERT INTO MacroActions(macro_id, \"order\", action_type, click_type, repeat, position, current_position,"
                 " interval, hold_duration, hover_duration, click_count, mouse_button)"
                 " VALUES(?,?,?,?,?,?,?,?,?,?,?,?)");
    insQ.addBindValue(newAction.macro_id);
    insQ.addBindValue(newAction.order);
    insQ.addBindValue(actionTypeToStr(newAction.action_type));
    insQ.addBindValue(clickTypeToStr(newAction.click_type));
    insQ.addBindValue(newAction.repeat);
    if (newAction.position.has_value()) insQ.addBindValue(newAction.position.value()); else insQ.addBindValue(QVariant(QVariant::String));
    insQ.addBindValue(newAction.current_pos ? 1 : 0);
    insQ.addBindValue(newAction.interval);
    if (newAction.hold_duration.has_value()) insQ.addBindValue(newAction.hold_duration.value()); else insQ.addBindValue(QVariant(QVariant::Int));
    if (newAction.hover_duration.has_value()) insQ.addBindValue(newAction.hover_duration.value()); else insQ.addBindValue(QVariant(QVariant::Int));
    insQ.addBindValue(newAction.click_count);
    if (newAction.mouse_button.has_value()) insQ.addBindValue(mouseButtonToStr(newAction.mouse_button.value())); else insQ.addBindValue(QVariant(QVariant::String));

    if (!insQ.exec()) {
        m_db.rollback();
        execQuery(insQ, "insert updated action", error);
        return false;
    }

    if (!m_db.commit()) {
        m_db.rollback();
        if (error) *error = "Commit failed";
        return false;
    }

    return true;
}

bool MacroManager::setActionsForMacro(int macroId, const QVector<MacroAction>& actions, QString* error) {
    if (!m_db.transaction()) { if (error) *error="transaction begin failed"; return false; }

    QSqlQuery del(m_db);
    del.prepare("DELETE FROM MacroActions WHERE macro_id=?");
    del.addBindValue(macroId);
    if (!del.exec()) { m_db.rollback(); return execQuery(del, "clear actions", error); }

    QSqlQuery ins(m_db);
    ins.prepare("INSERT INTO MacroActions(macro_id, \"order\", action_type, click_type, repeat, position, current_position,"
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

// ====== MIGRATION ==========
bool MacroManager::migrateToSequentialIds(QString* error) {
    Logger::instance().mInfo("Starting migration to sequential IDs");

    // Önce tüm açık query'leri temizle
    m_db.close();
    if (!m_db.open()) {
        if (error) *error = "Failed to reopen database for migration";
        return false;
    }

    // PRAGMA ayarları
    QSqlQuery pragmaQ(m_db);
    pragmaQ.exec("PRAGMA foreign_keys = OFF");  // Migration sırasında foreign key'leri kapat

    if (!m_db.transaction()) {
        if (error) *error = "Transaction failed";
        return false;
    }

    try {
        QSqlQuery q(m_db);

        // 1. Yeni tablo yapısını oluştur (farklı isimlerle)
        const char* createNewMacros =
            "CREATE TABLE Macros_new ("
            " id INTEGER PRIMARY KEY,"  // AUTOINCREMENT yok
            " name TEXT NOT NULL UNIQUE,"
            " description TEXT NOT NULL,"
            " hotkey TEXT NOT NULL"
            ");";
        if (!q.exec(createNewMacros)) {
            m_db.rollback();
            pragmaQ.exec("PRAGMA foreign_keys = ON");
            return execQuery(q, "create new macros table", error);
        }

        const char* createNewActions =
            "CREATE TABLE MacroActions_new ("
            " macro_id INTEGER NOT NULL,"
            " \"order\" INTEGER NOT NULL,"
            " action_type TEXT NOT NULL,"
            " click_type TEXT NOT NULL,"
            " repeat INTEGER NOT NULL,"
            " position TEXT NULL,"
            " current_position INTEGER NOT NULL,"
            " interval INTEGER NOT NULL,"
            " hold_duration INTEGER NULL,"
            " hover_duration INTEGER NULL,"
            " click_count INTEGER NOT NULL,"
            " mouse_button TEXT NULL,"
            " PRIMARY KEY(macro_id, \"order\"),"
            " FOREIGN KEY(macro_id) REFERENCES Macros_new(id) ON DELETE CASCADE"
            ");";
        if (!q.exec(createNewActions)) {
            m_db.rollback();
            pragmaQ.exec("PRAGMA foreign_keys = ON");
            return execQuery(q, "create new actions table", error);
        }

        // Debug: Mevcut tablo yapısını kontrol et
        if (!q.exec("PRAGMA table_info(MacroActions)")) {
            m_db.rollback();
            pragmaQ.exec("PRAGMA foreign_keys = ON");
            return execQuery(q, "get table info", error);
        }

        QStringList oldColumns;
        while (q.next()) {
            oldColumns << q.value(1).toString(); // column name
        }
        Logger::instance().mInfo(QString("Old MacroActions columns: %1").arg(oldColumns.join(", ")));

        // 2. Verileri kopyala
        if (!q.exec("INSERT INTO Macros_new (id, name, description, hotkey) "
                    "SELECT id, name, description, hotkey FROM Macros ORDER BY id")) {
            m_db.rollback();
            pragmaQ.exec("PRAGMA foreign_keys = ON");
            return execQuery(q, "copy macros data", error);
        }

        // MacroActions için sütunları explicit belirt
        if (!q.exec("INSERT INTO MacroActions_new (macro_id, \"order\", action_type, click_type, repeat, position, current_position, interval, hold_duration, hover_duration, click_count, mouse_button) "
                    "SELECT macro_id, \"order\", action_type, click_type, repeat, position, current_position, interval, hold_duration, hover_duration, click_count, mouse_button FROM MacroActions")) {
            m_db.rollback();
            pragmaQ.exec("PRAGMA foreign_keys = ON");
            return execQuery(q, "copy actions data", error);
        }

        // 3. Eski tabloları sil
        if (!q.exec("DROP TABLE MacroActions")) {
            m_db.rollback();
            pragmaQ.exec("PRAGMA foreign_keys = ON");
            return execQuery(q, "drop old actions", error);
        }

        if (!q.exec("DROP TABLE Macros")) {
            m_db.rollback();
            pragmaQ.exec("PRAGMA foreign_keys = ON");
            return execQuery(q, "drop old macros", error);
        }

        // 4. Yeni tabloları rename et
        if (!q.exec("ALTER TABLE Macros_new RENAME TO Macros")) {
            m_db.rollback();
            pragmaQ.exec("PRAGMA foreign_keys = ON");
            return execQuery(q, "rename macros table", error);
        }

        if (!q.exec("ALTER TABLE MacroActions_new RENAME TO MacroActions")) {
            m_db.rollback();
            pragmaQ.exec("PRAGMA foreign_keys = ON");
            return execQuery(q, "rename actions table", error);
        }

        if (!m_db.commit()) {
            m_db.rollback();
            pragmaQ.exec("PRAGMA foreign_keys = ON");
            if (error) *error = "Commit failed";
            return false;
        }

        // Foreign key'leri tekrar aç
        pragmaQ.exec("PRAGMA foreign_keys = ON");
        Logger::instance().mInfo("Migration completed successfully");
        return true;

    } catch (...) {
        m_db.rollback();
        pragmaQ.exec("PRAGMA foreign_keys = ON");
        if (error) *error = "Migration failed with exception";
        return false;
    }
}
