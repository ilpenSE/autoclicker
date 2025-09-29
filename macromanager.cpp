#include "macromanager.h"

#include <QDir>
#include <QRegularExpression>
#include <QSqlDriver>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QStandardPaths>
#include <QThread>
#include <QVariant>

#include "appdatamanager.h"
#include "LoggerStream.h"
#include "transactionguard.h"

static const int DEFAULT_MACRO_ID = 1;
static const char* CREATE_MACROS_SQL =
    "CREATE TABLE Macros ("
    " id INTEGER PRIMARY KEY,"
    " name TEXT NOT NULL UNIQUE,"
    " description TEXT NOT NULL,"
    " hotkey TEXT NOT NULL"
    ");";

static const char* CREATE_ACTIONS_SQL =
    "CREATE TABLE MacroActions ("
    " macro_id INTEGER NOT NULL,"
    " \"order\" INTEGER NOT NULL,"
    " action_type TEXT NOT NULL,"
    " click_type TEXT NOT NULL,"
    " repeat INTEGER NOT NULL,"
    " position TEXT NULL,"
    " current_position INTEGER NOT NULL,"
    " interval INTEGER NOT NULL,"
    " hold_duration INTEGER NULL,"
    " click_count INTEGER NOT NULL,"
    " mouse_button TEXT NULL,"
    " key_name TEXT NULL,"
    " PRIMARY KEY(macro_id, \"order\"),"
    " FOREIGN KEY(macro_id) REFERENCES Macros(id) ON DELETE CASCADE"
    ");";
// QUERY BIND HELPERS
static inline void addBindOptional(QSqlQuery& q, const std::optional<QString>& v) {
  if (v.has_value()) q.addBindValue(v.value());
  else q.addBindValue(QVariant()); // explicit NULL
}

static inline void addBindOptional(QSqlQuery& q, const std::optional<int>& v) {
  if (v.has_value()) q.addBindValue(v.value());
  else q.addBindValue(QVariant());
}

static inline void bindActionToQuery(QSqlQuery& q, const MacroAction& a, bool includeKeys = false) {
  if (includeKeys) {
    q.addBindValue(a.macro_id);
    q.addBindValue(a.order);
  }

  q.addBindValue(a.macro_id);
  q.addBindValue(a.order);
  q.addBindValue(actionTypeToStr(a.action_type));
  q.addBindValue(clickTypeToStr(a.click_type));
  q.addBindValue(a.repeat);
  addBindOptional(q, a.position);
  if (a.current_pos.has_value()) q.addBindValue(a.current_pos.value() ? 1 : 0);
  else q.addBindValue(QVariant());
  q.addBindValue(a.interval);
  addBindOptional(q, a.hold_duration);
  q.addBindValue(a.click_count);
  if (a.action_type == ActionType::KEYBOARD || !a.mouse_button.has_value())
    q.addBindValue(QVariant());
  else
    q.addBindValue(mouseButtonToStr(a.mouse_button.value()));
  addBindOptional(q, a.key_name);
}

static inline void bindMacroToQuery(QSqlQuery& q, const Macro& m, bool includeId = false) {
  if (includeId) {
    q.addBindValue(m.id);
  }

  q.addBindValue(m.name);
  q.addBindValue(m.description);
  q.addBindValue(m.hotkey);
}

bool MacroManager::updateOrder(int macroId, int oldOrder, int newOrder,
                               const char* step, QString* error) {
  QSqlQuery q(m_db);
  q.prepare("UPDATE MacroActions SET \"order\"=? WHERE macro_id=? AND \"order\"=?");
  q.addBindValue(newOrder);
  q.addBindValue(macroId);
  q.addBindValue(oldOrder);

  if (!q.exec()) {
    return execQuery(q, step, error);
  }
  return true;
}

QString MacroManager::dbPath() const {
  const auto base = AppDataManager::instance().appFolderPath();
  QDir().mkpath(base);
  return QDir(base).filePath("macros.sqlite");
}

bool MacroManager::init() {
  if (m_db.isValid() && m_db.isOpen()) return true;

  const QString path = dbPath();
  m_db = QSqlDatabase::addDatabase("QSQLITE", "macros_conn");
  m_db.setDatabaseName(path);
  if (!isOpen()) return false;

  QString err;
  if (!ensureSchema(&err)) {
    mwrn() << err;
    return false;
  }
  if (!ensureDefaultMacro(&err)) {
    merr() << err;
    return false;
  }
  return true;
}

bool MacroManager::isOpen() const { return m_db.isOpen(); }

bool MacroManager::ensureSchema(QString* error) {
  QSqlQuery q(m_db);

  // Enable foreign keys
  if (!q.exec("PRAGMA foreign_keys = ON;"))
    return execQuery(q, "PRAGMA", error);

  // Check if tables exist and validate their schemas
  if (!validateAndCreateMacrosTable(error)) return false;
  if (!validateAndCreateActionsTable(error)) return false;

  return true;
}

bool MacroManager::ensureDefaultMacro(QString* error) {
  TransactionGuard tx(m_db);
  if (!tx.ok()) return false;

  QSqlQuery q(m_db);
  if (!q.exec("SELECT COUNT(*) FROM Macros;"))
    return execQuery(q, "count Macros", error);
  int cnt = 0;
  if (q.next()) cnt = q.value(0).toInt();
  if (cnt > 0) return true;

  QSqlQuery insM(m_db);
  insM.prepare(
      "INSERT INTO Macros(id, name, description, hotkey) VALUES(?, ?, ?, ?)");
  bindMacroToQuery(insM, defaultMacro, true);
  if (!insM.exec()) {
    return execQuery(insM, "insert DEFAULT Macro", error);
  }

  QSqlQuery insA(m_db);
  insA.prepare(
      "INSERT INTO MacroActions("
      "macro_id, \"order\", action_type, click_type, repeat, position, "
      "current_position,"
      " interval, hold_duration, click_count, mouse_button, "
      "key_name)"
      " VALUES(?,?,?,?,?,?,?,?,?,?,?,?)");
  bindActionToQuery(insA, defaultAction, true);
  if (!insA.exec()) {
    return execQuery(insA, "insert DEFAULT Action", error);
  }

  return tx.commit();
}

bool MacroManager::execQuery(QSqlQuery& q, const char* ctx,
                             QString* error) const {
  if (error) *error = QString("%1: %2").arg(ctx).arg(q.lastError().text());
  return false;
}

// ===== Enchanced Validation =====
bool MacroManager::validateAndCreateMacrosTable(QString* error) {
  QSqlQuery q(m_db);

  // Check if Macros table exists
  if (!q.exec("SELECT name FROM sqlite_master WHERE type='table' AND "
              "name='Macros'")) {
    return execQuery(q, "check Macros table", error);
  }

  bool tableExists = q.next();
  q.finish();

  if (!tableExists) {
    // Create new table
    if (!q.exec(CREATE_MACROS_SQL)) return execQuery(q, "create Macros", error);
    minfo() << "Created new Macros table";
    return true;
  }

  // Validate existing table schema
  if (!q.exec("PRAGMA table_info(Macros)")) {
    return execQuery(q, "get Macros schema", error);
  }

  QStringList expectedColumns = {"id", "name", "description", "hotkey"};
  QStringList actualColumns;

  while (q.next()) {
    actualColumns.append(q.value(1).toString());  // column name
  }
  q.finish();

  // Check if schema matches
  if (actualColumns.size() != expectedColumns.size()) {
    mwrn() << "Macros table schema mismatch - recreating";
    return recreateMacrosTable(error);
  }

  for (const QString& col : expectedColumns) {
    if (!actualColumns.contains(col)) {
      mwrn() << "Missing column in Macros: " + col;
      return recreateMacrosTable(error);
    }
  }

  minfo() << "Macros table schema validated";
  return true;
}

bool MacroManager::validateAndCreateActionsTable(QString* error) {
  QSqlQuery q(m_db);

  // Check if MacroActions table exists
  if (!q.exec("SELECT name FROM sqlite_master WHERE type='table' AND "
              "name='MacroActions'")) {
    return execQuery(q, "check MacroActions table", error);
  }

  bool tableExists = q.next();
  q.finish();

  if (!tableExists) {
    // Create new table
    if (!q.exec(CREATE_ACTIONS_SQL))
      return execQuery(q, "create MacroActions", error);
    minfo() << "Created new MacroActions table";
    return true;
  }

  // Validate existing table schema
  if (!q.exec("PRAGMA table_info(MacroActions)")) {
    return execQuery(q, "get MacroActions schema", error);
  }

  QStringList expectedColumns = {
      "macro_id",      "order",       "action_type",      "click_type",
      "repeat",        "position",    "current_position", "interval",
      "hold_duration", "click_count", "mouse_button",     "key_name"};

  QStringList actualColumns;
  QSet<QString> actualColumnSet;

  while (q.next()) {
    QString colName = q.value(1).toString();
    actualColumns.append(colName);
    actualColumnSet.insert(colName);
  }
  q.finish();

  // Check for missing expected columns
  bool hasMissingColumns = false;
  for (const QString& col : expectedColumns) {
    if (!actualColumnSet.contains(col)) {
      mwrn() << "Missing column in MacroActions: " + col;
      hasMissingColumns = true;
    }
  }

  // Recreate table if schema doesn't match
  if (hasMissingColumns) {
    mwrn() << "MacroActions table schema mismatch - recreating";
    return recreateActionsTable(error);
  }

  minfo() << "MacroActions table schema validated";
  return true;
}

bool MacroManager::recreateMacrosTable(QString* error) {
  TransactionGuard tx(m_db);
  if (!tx.ok()) {
    if (error) *error = "Transaction guard failed!";
    return false;
  }

  QSqlQuery q(m_db);

  // Backup existing data
  if (!q.exec("CREATE TEMP TABLE macros_backup AS SELECT * FROM Macros")) {
    return execQuery(q, "backup Macros", error);
  }

  // Drop old table
  if (!q.exec("DROP TABLE Macros")) {
    return execQuery(q, "drop old Macros", error);
  }

  // Create new table
  if (!q.exec(CREATE_MACROS_SQL)) {
    return execQuery(q, "recreate Macros", error);
  }

  // Restore data
  if (!q.exec("INSERT INTO Macros (id, name, description, hotkey) "
              "SELECT id, name, description, hotkey FROM macros_backup")) {
    return execQuery(q, "restore Macros data", error);
  }

  // Clean up
  if (!q.exec("DROP TABLE macros_backup")) {
    return execQuery(q, "cleanup Macros backup", error);
  }

  minfo() << "Successfully recreated Macros table";
  return tx.commit();
}

bool MacroManager::recreateActionsTable(QString* error) {
  TransactionGuard tx(m_db);
  if (!tx.ok()) {
    if (error) *error = "Transaction guard failed!";
    return false;
  }

  QSqlQuery q(m_db);

  // Backup existing data with column mapping
  QString backupQuery =
      "CREATE TEMP TABLE actions_backup AS SELECT "
      "macro_id, \"order\", action_type, click_type, repeat, "
      "position, current_position, interval, "
      "hold_duration, click_count, mouse_button, key_name "
      "FROM MacroActions";

  if (!q.exec(backupQuery)) {
    return execQuery(q, "backup MacroActions", error);
  }

  // Drop old table
  if (!q.exec("DROP TABLE MacroActions")) {
    return execQuery(q, "drop old MacroActions", error);
  }

  // Create new table with correct schema
  if (!q.exec(CREATE_ACTIONS_SQL)) {
    return execQuery(q, "recreate MacroActions", error);
  }

  // Restore data
  QString restoreQuery =
      "INSERT INTO MacroActions ("
      "macro_id, \"order\", action_type, click_type, repeat, "
      "position, current_position, interval, "
      "hold_duration, click_count, mouse_button, key_name) "
      "SELECT macro_id, \"order\", action_type, click_type, repeat, "
      "position, current_position, interval, "
      "hold_duration, click_count, mouse_button, key_name "
      "FROM actions_backup";

  if (!q.exec(restoreQuery)) {
    return execQuery(q, "restore MacroActions data", error);
  }

  // Clean up
  if (!q.exec("DROP TABLE actions_backup")) {
    return execQuery(q, "cleanup MacroActions backup", error);
  }

  minfo() << "Successfully recreated MacroActions table";
  return tx.commit();
}

// ===== Basic Validation =====
bool MacroManager::validateMacroName(const QString& name,
                                     QString* error) const {
  // 1-20 chars, letters/digits/_/-/space
  static QRegularExpression rx(R"(^[\p{L}\p{N} _-]{1,20}$)");
  if (!rx.match(name).hasMatch()) {
    if (error)
      *error =
          "Name length can be between 1-20 and only contains A-Z a-z 0-9 _ - "
          "and space!";
    return false;
  }

  return true;
}
bool MacroManager::validateMacroDescription(const QString& desc,
                                            QString* error) const {
  static QRegularExpression rx(
      R"(^[\p{L}\p{N} _-]{1,100}$)");  // kiril alfabesi gibi é, ç, ı gibi
                                       // ingilizce olmayan harfleri de
                                       // destekler
  if (!rx.match(desc).hasMatch()) {
    if (error)
      *error =
          "Description can be between 1-100 and only contains A-Z a-z 0-9 _ - "
          "and space!";
    return false;
  }
  return true;
}
bool MacroManager::validateHotkey(const QString& hotkey, QString* error) const {
  // "DEF" or "[mod + ]KEY"  (mods: Ctrl|Shift|Alt), disallow LWin|RWin|Fn|Enter
  if (hotkey == "DEF") return true;

  // Examples allowed: "F7", "Shift + F6", "Ctrl + Alt + A"
  static QRegularExpression rx(
      R"(^\s*(?:(?:Ctrl|Shift|Alt)\s*\+\s*){0,3}(?:F([1-9]|1[0-9]|2[0-4])|[A-Z0-9]|Space|Tab|Backspace|Delete|Insert|Home|End|PageUp|PageDown|Up|Down|Left|Right)\s*$)",
      QRegularExpression::CaseInsensitiveOption);

  if (!rx.match(hotkey).hasMatch()) {
    if (error) *error = "Invalid hotkey!";
    return false;
  }
  // guard forbidden keys
  if (hotkey.contains("LWin", Qt::CaseInsensitive) ||
      hotkey.contains("RWin", Qt::CaseInsensitive) ||
      hotkey.contains("Fn", Qt::CaseInsensitive) ||
      hotkey.compare("Enter", Qt::CaseInsensitive) == 0) {
    if (error) *error = "Hotkey can't contain LWin, RWin, Fn, Enter.";
    return false;
  }
  return true;
}

template <typename T, typename... Args>
bool equalsAtLeastOne(const T& query, const Args&... values) {
  return ((query == values) || ...);  // C++17 fold expression
}
bool MacroManager::validateAction(const MacroAction& a, QString* error) const {
  if (a.macro_id <= 0) {
    if (error) *error = "invalid macro_id.";
    return false;
  }

  if (!equalsAtLeastOne(a.action_type, ActionType::KEYBOARD,
                        ActionType::MOUSE)) {
    if (error) *error = "action_type must be 'mouse' or 'keyboard'.";
    return false;
  }

  if (!equalsAtLeastOne(a.click_type, ClickType::CLICK, ClickType::HOLD,
                        ClickType::HOVER)) {
    if (error) *error = "click_type must be CLICK/HOLD/HOVER.";
    return false;
  }

  // Strict validation for KEYBOARD actions
  if (a.action_type == ActionType::KEYBOARD) {
    if (a.click_type == ClickType::HOVER) {
      if (error) *error = "KEYBOARD actions cannot use HOVER click type.";
      return false;
    }
    if (a.mouse_button.has_value()) {
      if (error)
        *error = "KEYBOARD actions cannot have mouse button specified.";
      return false;
    }
    if (!a.key_name.has_value() || a.key_name->isEmpty()) {
      if (error) *error = "KEYBOARD actions must specify a key_name.";
      return false;
    }
  }

  // Validation for MOUSE actions
  if (a.action_type == ActionType::MOUSE) {
    if (!a.mouse_button.has_value()) {
      if (error) *error = "MOUSE actions must specify a mouse button.";
      return false;
    }
  }

  if (a.repeat < 0) {
    if (error) *error = "Repeat can't be negative (0 = infinite).";
    return false;
  }
  if (a.interval < 0) {
    if (error) *error = "interval can't be negative.";
    return false;
  }
  if (a.click_count < 1) {
    if (error) *error = "click_count must be at least 1.";
    return false;
  }

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
  m.id = q.value(0).toInt();
  m.name = q.value(1).toString();
  m.description = q.value(2).toString();
  m.hotkey = q.value(3).toString();
  return m;
}

int MacroManager::createMacro(const Macro& m, QString* error) {
  if (!validateMacroName(m.name, error) ||
      !validateMacroDescription(m.description, error) ||
      !validateHotkey(m.hotkey, error))
    return -1;

  TransactionGuard tx(m_db);
  if (!tx.ok()) {
    if (error) *error = "Transaction guard failed!";
    return -1;
  }

  if (existsMacro(m.name)) {
    if (error) *error = "A macro with this name exists!";
    return -1;
  }

  QSqlQuery q(m_db);
  q.prepare("INSERT INTO Macros(name, description, hotkey) VALUES(?,?,?)");
  bindMacroToQuery(q, m);
  if (!q.exec()) {
    execQuery(q, "insert Macro", error);
    return -1;
  }

  if (!tx.commit()) {
    if (error) *error = "Create macro commit failed!";
    return -1;
  }
  return q.lastInsertId().toInt();
}

bool MacroManager::updateMacro(const Macro& m, QString* error) {
  if (m.id == DEFAULT_MACRO_ID) {
    if (error) *error = "DEFAULT cannot be edited.";
    return false;
  }
  if (!validateMacroName(m.name, error) ||
      !validateMacroDescription(m.description, error) ||
      !validateHotkey(m.hotkey, error))
    return false;

  TransactionGuard tx(m_db);
  if (!tx.ok()) {
    if (error) *error = "Transaction failed!";
    return false;
  }

  QSqlQuery q(m_db);
  q.prepare("UPDATE Macros SET name=?, description=?, hotkey=? WHERE id=?");
  q.addBindValue(m.name);
  q.addBindValue(m.description);
  q.addBindValue(m.hotkey);
  q.addBindValue(m.id);
  if (!q.exec()) return execQuery(q, "update Macro", error);

  return tx.commit();
}

bool MacroManager::deleteMacro(int id, QString* error) {
  if (id == DEFAULT_MACRO_ID) {
    if (error) *error = "DEFAULT cannot be deleted.";
    return false;
  }
  QSqlQuery q(m_db);
  q.prepare("DELETE FROM Macros WHERE id=?");
  q.addBindValue(id);
  if (!q.exec()) return execQuery(q, "delete Macro", error);
  return true;
}
// MacroManager'ın cpp dosyasına ekleyin:

bool MacroManager::updateMacroName(int macroId, const QString& newName,
                                   QString* error) {
  if (macroId == DEFAULT_MACRO_ID) {
    if (error) *error = "DEFAULT cannot be edited.";
    return false;
  }

  if (!validateMacroName(newName, error)) {
    return false;
  }

  // Aynı isimde başka macro var mı kontrol et (kendisi hariç)
  QSqlQuery checkQuery(m_db);
  checkQuery.prepare("SELECT id FROM Macros WHERE name=? AND id!=?");
  checkQuery.addBindValue(newName);
  checkQuery.addBindValue(macroId);
  if (checkQuery.exec() && checkQuery.next()) {
    if (error) *error = "A macro with this name exists!";
    return false;
  }

  if (existsMacro(macroId))

  QSqlQuery q(m_db);
  q.prepare("UPDATE Macros SET name=? WHERE id=?");
  q.addBindValue(newName);
  q.addBindValue(macroId);

  if (!q.exec()) {
    return execQuery(q, "update macro name", error);
  }
  return true;
}

bool MacroManager::updateMacroDescription(int macroId,
                                          const QString& newDescription,
                                          QString* error) {
  if (macroId == DEFAULT_MACRO_ID) {
    if (error) *error = "DEFAULT cannot be edited.";
    return false;
  }

  if (!validateMacroDescription(newDescription, error)) {
    return false;
  }

  QSqlQuery q(m_db);
  q.prepare("UPDATE Macros SET description=? WHERE id=?");
  q.addBindValue(newDescription);
  q.addBindValue(macroId);

  if (!q.exec()) {
    return execQuery(q, "update macro description", error);
  }
  return true;
}

bool MacroManager::updateMacroHotkey(int macroId, const QString& newHotkey,
                                     QString* error) {
  if (macroId == DEFAULT_MACRO_ID) {
    if (error) *error = "DEFAULT cannot be edited.";
    return false;
  }

  if (!validateHotkey(newHotkey, error)) {
    return false;
  }

  QSqlQuery q(m_db);
  q.prepare("UPDATE Macros SET hotkey=? WHERE id=?");
  q.addBindValue(newHotkey);
  q.addBindValue(macroId);

  if (!q.exec()) {
    return execQuery(q, "update macro hotkey", error);
  }
  return true;
}

// ===== Actions CRUD =====
QVector<MacroAction> MacroManager::getActions(int macroId) const {
  QVector<MacroAction> out;
  static const QString sql =
      "SELECT macro_id, \"order\", action_type, click_type, repeat, "
      "position, current_position, interval, hold_duration, click_count, "
      "mouse_button, key_name FROM MacroActions WHERE macro_id=? "
      "ORDER BY \"order\" ASC";

  QSqlQuery q(m_db);
  q.prepare(sql);
  q.addBindValue(macroId);
  if (!q.exec()) return out;

  while (q.next()) {
    MacroAction a;
    a.macro_id = q.value(0).toInt();
    a.order = q.value(1).toInt();
    a.repeat = q.value(4).toInt();
    a.position = q.isNull(5) ? std::optional<QString>{}
                             : std::optional<QString>{q.value(5).toString()};
    a.current_pos = q.isNull(6) ? std::optional<bool>{}
                                : std::optional<bool>{q.value(6).toInt() != 0};
    a.interval = q.value(7).toInt();
    a.hold_duration = q.isNull(8) ? std::optional<int>{}
                                  : std::optional<int>{q.value(8).toInt()};
    a.click_count = q.value(9).toInt();
    a.key_name = q.value(11).toString();

    // Enum casting'ler
    QString strAct = q.value(2).toString();
    auto actOpt = strToActionType(strAct);
    if (!actOpt) {
      merr() << "Invalid action type: " + strAct +
                                " for macro " + QString::number(a.macro_id) +
                                " order " + QString::number(a.order);
      continue;
    }
    a.action_type = *actOpt;

    QString strClick = q.value(3).toString();
    auto clickOpt = strToClickType(strClick);
    if (!clickOpt) {
      merr() << "Invalid click type: " + strClick +
                                " for macro " + QString::number(a.macro_id) +
                                " order " + QString::number(a.order);
      continue;
    }
    a.click_type = *clickOpt;

    if (!q.isNull(10)) {
      auto btnOpt = strToMouseButton(q.value(10).toString());
      if (btnOpt) {
        a.mouse_button = *btnOpt;
      }
    }

    out.push_back(std::move(a));
  }
  return out;
}
bool MacroManager::addAction(const MacroAction& action, QString* error) {
  static const char* INSERT_SQL = R"(
        INSERT INTO MacroActions (
            macro_id, "order", action_type, click_type, repeat,
            position, current_position, interval, hold_duration,
            click_count, mouse_button, key_name
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )";

  QSqlQuery insQ(m_db);
  if (!insQ.prepare(INSERT_SQL)) {
    *error = "Failed to prepare insert: " + insQ.lastError().text();
    return false;
  }

  TransactionGuard tx(m_db);
  if (!tx.ok()) { *error = "Transaction guard failed!"; return false; }

  bindActionToQuery(insQ, action);
  if (!insQ.exec()) {
    *error = "Insert failed: " + insQ.lastError().text();
    return false;
  }

  return tx.commit();
}

bool MacroManager::existsAction(int macro_id, int order, QString* error) {
  QSqlQuery checkQ(m_db);
  checkQ.prepare(
      "SELECT COUNT(*) FROM MacroActions WHERE macro_id=? AND \"order\"=?");
  checkQ.addBindValue(macro_id);
  checkQ.addBindValue(order);

  if (!checkQ.exec()) {
    if (error)
      *error = QString("Failed to check action existence: %1")
                   .arg(checkQ.lastError().text());
    return false;
  }

  bool actionExists = false;
  if (checkQ.next()) {
    actionExists = (checkQ.value(0).toInt() > 0);
  }
  checkQ.finish();

  return actionExists;
}

bool MacroManager::deleteAction(int macroId, int order, QString* error) {
  // Basic input validation
  if (macroId <= 0) {
    if (error) *error = "Invalid macro ID";
    return false;
  }

  if (order < 0) {
    if (error) *error = "Invalid order";
    return false;
  }

  // Start transaction
  TransactionGuard tx(m_db);
  if (!tx.ok()) {
      if (error) *error = "Transaction guard failed!";
      return false;
    }

  // Delete the specific action
  QSqlQuery delQ(m_db);
  delQ.prepare("DELETE FROM MacroActions WHERE macro_id=? AND \"order\"=?");
  delQ.addBindValue(macroId);
  delQ.addBindValue(order);

  if (!delQ.exec()) {
    QString deleteError =
        QString("Failed to delete action: %1").arg(delQ.lastError().text());
    delQ.finish();
    if (error) *error = deleteError;
    return false;
  }
  delQ.finish();

  // Shift down all actions with higher order numbers
  QSqlQuery shiftQ(m_db);
  shiftQ.prepare(
      "UPDATE MacroActions SET \"order\" = \"order\" - 1 WHERE macro_id=? AND "
      "\"order\" > ?");
  shiftQ.addBindValue(macroId);
  shiftQ.addBindValue(order);

  if (!shiftQ.exec()) {
    QString shiftError = QString("Failed to shift action orders: %1")
                             .arg(shiftQ.lastError().text());
    shiftQ.finish();
    if (error) *error = shiftError;
    return false;
  }
  shiftQ.finish();

  return tx.commit();
}
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
  if (!maxQ.exec()) {
    return execQuery(maxQ, "get max order", error);
  }
  if (!maxQ.next()) {
    if (error) *error = "No actions found for this macro";
    return false;
  }
  int maxOrder = maxQ.value(0).toInt();

  if (order >= maxOrder) {
    if (error) *error = "Already at bottom position";
    return false;
  }
  return swapActions(macroId, order, order + 1, error);
}

// İki action'ın yerlerini değiştir
bool MacroManager::swapActions(int macroId, int order1, int order2,
                               QString* error) {
  TransactionGuard tx(m_db);
  if (!tx.ok()) {
    if (error) *error = "Transaction guard failed!";
    return false;
  }

  if (!updateOrder(macroId, order1, tempOrder, "move to temp", error)) return false;
  if (!updateOrder(macroId, order2, order1, "move second", error)) return false;
  if (!updateOrder(macroId, tempOrder, order2, "move from temp", error)) return false;

  return tx.commit();
}

bool MacroManager::updateAction(int macroId, int oldOrder,
                                const MacroAction& newAction, QString* error) {
  if (!validateAction(newAction, error)) return false;

  TransactionGuard tx(m_db);
  if (!tx.ok()) {
    if (error) *error = "Transaction guard failed!";
    return false;
  }

  // Delete old action
  QSqlQuery delQ(m_db);
  delQ.prepare("DELETE FROM MacroActions WHERE macro_id=? AND \"order\"=?");
  delQ.addBindValue(macroId);
  delQ.addBindValue(oldOrder);
  if (!delQ.exec()) {
    return execQuery(delQ, "delete old action", error);
  }

  // Insert new action
  QSqlQuery insQ(m_db);
  insQ.prepare(
      "INSERT INTO MacroActions(macro_id, \"order\", action_type, click_type, "
      "repeat, position, current_position,"
      " interval, hold_duration, click_count, mouse_button, "
      "key_name)"
      " VALUES(?,?,?,?,?,?,?,?,?,?,?,?)");
  bindActionToQuery(insQ, newAction, true);

  if (!insQ.exec()) {
    return execQuery(insQ, "insert updated action", error);
  }

  return tx.commit();
}

bool MacroManager::setActionsForMacro(int macroId,
                                      const QVector<MacroAction>& actions,
                                      QString* error) {
  TransactionGuard tx(m_db);
  if (!tx.ok()) { *error = "Transaction guard failed!"; return false; }

  // eski aksiyonları sil
  QSqlQuery delQ(m_db);
  delQ.prepare("DELETE FROM MacroActions WHERE macro_id = ?");
  delQ.addBindValue(macroId);
  if (!delQ.exec()) {
    merr() << "Failed to delete old actions: " << delQ.lastError().text();
    return false;
  }

  // Yeni aksiyonları ekle
  QSqlQuery insQ(m_db);
  insQ.prepare(R"(
        INSERT INTO MacroActions (
            macro_id, "order", action_type, click_type, repeat,
            position, current_position, interval, hold_duration,
            click_count, mouse_button, key_name
        ) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)
    )");

  for (const auto& a : actions) {
    bindActionToQuery(insQ, a);
    if (!insQ.exec()) {
      merr() << "Insert failed: " << insQ.lastError().text();
      return false;
    }
  }

  return tx.commit();
}
bool MacroManager::normalizeOrders(int macroId, QString* error) {
  auto acts = getActions(macroId);
  if (acts.isEmpty()) return true;
  for (int i = 0; i < acts.size(); ++i) acts[i].order = i;
  return setActionsForMacro(macroId, acts, error);
}
