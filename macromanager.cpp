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
#include "logger.h"

static const int DEFAULT_MACRO_ID = 1;

MacroManager& MacroManager::instance() {
  static MacroManager inst;
  return inst;
}

MacroManager::MacroManager(QObject* parent) : QObject(parent) {}

QString MacroManager::dbPath() const {
  const auto base = AppDataManager::instance().appFolderPath();
  // const auto base =
  // QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
  // AppDataLocation -> %APPDATA%/AutoClicker2 (Windows),
  // ~/.local/share/AutoClicker2 (Linux)
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
  if (!ensureSchema(&err)) {
    Logger::instance().mWarning("ensureSchema failed:" + err);
    return false;
  }
  if (!ensureDefaultMacro(&err)) {
    Logger::instance().mError("ensureDefaultMacro failed:" + err);
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

  // Migration kontrolü - eğer tablo AUTOINCREMENT ile varsa migrate et
  QSqlQuery checkQ(m_db);
  if (!checkQ.exec("SELECT name FROM sqlite_master WHERE type='table' AND "
                   "name='Macros'")) {
    return execQuery(checkQ, "check table exists", error);
  }

  bool tableExists = checkQ.next();
  checkQ.finish();  // Query'yi kapat

  if (!tableExists) {
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
      " key_name TEXT NULL,"
      " PRIMARY KEY(macro_id, \"order\"),"
      " FOREIGN KEY(macro_id) REFERENCES Macros(id) ON DELETE CASCADE"
      ");";
  if (!q.exec(createActions)) return execQuery(q, "create MacroActions", error);

  return true;
}
bool MacroManager::ensureDefaultMacro(QString* error) {
  QSqlQuery q(m_db);
  if (!q.exec("SELECT COUNT(*) FROM Macros;"))
    return execQuery(q, "count Macros", error);
  int cnt = 0;
  if (q.next()) cnt = q.value(0).toInt();
  if (cnt > 0) return true;

  // Insert DEFAULT macro and single default action inside a transaction
  if (!m_db.transaction()) return false;

  QSqlQuery insM(m_db);
  insM.prepare(
      "INSERT INTO Macros(id, name, description, hotkey) VALUES(?, ?, ?, ?)");
  insM.addBindValue(DEFAULT_MACRO_ID);
  insM.addBindValue("DEFAULT");
  insM.addBindValue("The defaults.");
  insM.addBindValue("DEF");
  if (!insM.exec()) {
    m_db.rollback();
    return execQuery(insM, "insert DEFAULT Macro", error);
  }

  QSqlQuery insA(m_db);
  insA.prepare(
      "INSERT INTO MacroActions("
      "macro_id, \"order\", action_type, click_type, repeat, position, "
      "current_position,"
      " interval, hold_duration, hover_duration, click_count, mouse_button, "
      "key_name)"
      " VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)");
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
  insA.addBindValue("A");
  if (!insA.exec()) {
    m_db.rollback();
    return execQuery(insA, "insert DEFAULT Action", error);
  }

  if (!m_db.commit()) {
    m_db.rollback();
    return false;
  }
  return true;
}

bool MacroManager::execQuery(QSqlQuery& q, const char* ctx,
                             QString* error) const {
  if (error) *error = QString("%1: %2").arg(ctx).arg(q.lastError().text());
  return false;
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
      R"(^(?:(?:Ctrl|Shift|Alt)\s*\+\s*){0,3}(?:F([1-9]|1[0-9]|2[0-4])|[A-Z0-9])$)");
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

  // ENHANCED: Strict validation for KEYBOARD actions
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

  // ENHANCED: Validation for MOUSE actions
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

int MacroManager::createMacro(const QString& name, const QString& description,
                              const QString& hotkey, QString* error) {
  if (!validateMacroName(name, error) ||
      !validateMacroDescription(description, error) ||
      !validateHotkey(hotkey, error))
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
  if (m.id == DEFAULT_MACRO_ID) {
    if (error) *error = "DEFAULT cannot be edited.";
    return false;
  }
  if (!validateMacroName(m.name, error) ||
      !validateMacroDescription(m.description, error) ||
      !validateHotkey(m.hotkey, error))
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

// ===== Actions CRUD =====
QVector<MacroAction> MacroManager::getActions(int macroId) const {
  QVector<MacroAction> out;
  QSqlQuery q(m_db);
  q.prepare(
      "SELECT macro_id, \"order\", action_type, click_type, repeat, position, "
      "current_position,"
      " interval, hold_duration, hover_duration, click_count, mouse_button, "
      "key_name "
      "FROM MacroActions WHERE macro_id=? ORDER BY \"order\" ASC");
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
    a.hover_duration = q.isNull(9) ? std::optional<int>{}
                                   : std::optional<int>{q.value(9).toInt()};
    a.click_count = q.value(10).toInt();
    a.key_name = q.value(12).toString();

    // Enum casting'ler
    QString strAct = q.value(2).toString();
    auto actOpt = strToActionType(strAct);
    if (!actOpt) {
      Logger::instance().mError("Invalid action type: " + strAct +
                                " for macro " + QString::number(a.macro_id) +
                                " order " + QString::number(a.order));
      continue;
    }
    a.action_type = *actOpt;

    QString strClick = q.value(3).toString();
    auto clickOpt = strToClickType(strClick);
    if (!clickOpt) {
      Logger::instance().mError("Invalid click type: " + strClick +
                                " for macro " + QString::number(a.macro_id) +
                                " order " + QString::number(a.order));
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
  // Create a copy for validation and auto-correction
  MacroAction correctedAction = a;

  // Auto-correct KEYBOARD actions
  if (correctedAction.action_type == ActionType::KEYBOARD) {
    correctedAction.mouse_button = std::nullopt;
    if (correctedAction.click_type == ClickType::HOVER) {
      correctedAction.click_type = ClickType::CLICK;
    }
    // Ensure key_name is set for keyboard actions
    if (!correctedAction.key_name.has_value()) {
      correctedAction.key_name = "A";  // Default key
    }
  }

  if (!validateAction(correctedAction, error)) return false;

  Logger::instance().mInfo(
      QString("Adding action: macro_id=%1, action_type=%2")
          .arg(correctedAction.macro_id)
          .arg(actionTypeToStr(correctedAction.action_type)));

  // Enhanced database state checking
  if (!m_db.isOpen()) {
    if (error) *error = "Database is not open";
    return false;
  }

  // Check if already in transaction
  if (m_db.driver()->hasFeature(QSqlDriver::Transactions)) {
    // Force rollback any existing transaction to clean state
    m_db.rollback();
  }

  if (!m_db.transaction()) {
    if (error)
      *error = QString("Transaction failed: %1").arg(m_db.lastError().text());
    return false;
  }

  // Get next order number with explicit query finish
  QSqlQuery maxQ(m_db);
  maxQ.prepare(
      "SELECT IFNULL(MAX(\"order\"), -1) + 1 FROM MacroActions WHERE "
      "macro_id=?");
  maxQ.addBindValue(correctedAction.macro_id);
  if (!maxQ.exec() || !maxQ.next()) {
    maxQ.finish();  // Explicitly finish query
    m_db.rollback();
    return execQuery(maxQ, "get max order", error);
  }
  int targetOrder = maxQ.value(0).toInt();
  maxQ.finish();  // Explicitly finish query

  Logger::instance().mInfo(QString("Auto-assigned order: %1").arg(targetOrder));

  // Insert new action with prepared statement
  QSqlQuery insQ(m_db);
  insQ.prepare(
      "INSERT INTO MacroActions(macro_id, \"order\", action_type, click_type, "
      "repeat, position, current_position, interval, hold_duration, "
      "hover_duration, "
      "click_count, mouse_button, key_name) VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)");

  insQ.addBindValue(correctedAction.macro_id);
  insQ.addBindValue(targetOrder);
  insQ.addBindValue(actionTypeToStr(correctedAction.action_type));
  insQ.addBindValue(clickTypeToStr(correctedAction.click_type));
  insQ.addBindValue(correctedAction.repeat);

  // Handle NULL values explicitly
  if (correctedAction.position.has_value()) {
    insQ.addBindValue(correctedAction.position.value());
  } else {
    insQ.addBindValue(QVariant(QVariant::String));
  }

  if (correctedAction.current_pos.has_value()) {
    insQ.addBindValue(correctedAction.current_pos.value() ? 1 : 0);
  } else {
    insQ.addBindValue(QVariant(QVariant::Int));
  }

  insQ.addBindValue(correctedAction.interval);

  if (correctedAction.hold_duration.has_value()) {
    insQ.addBindValue(correctedAction.hold_duration.value());
  } else {
    insQ.addBindValue(QVariant(QVariant::Int));
  }

  if (correctedAction.hover_duration.has_value()) {
    insQ.addBindValue(correctedAction.hover_duration.value());
  } else {
    insQ.addBindValue(QVariant(QVariant::Int));
  }

  insQ.addBindValue(correctedAction.click_count);

  // FIXED: Explicit NULL handling for KEYBOARD actions
  if (correctedAction.action_type == ActionType::KEYBOARD ||
      !correctedAction.mouse_button.has_value()) {
    insQ.addBindValue(QVariant(QVariant::String));  // Explicit NULL
  } else {
    insQ.addBindValue(mouseButtonToStr(correctedAction.mouse_button.value()));
  }

  if (correctedAction.key_name.has_value()) {
    insQ.addBindValue(correctedAction.key_name.value());
  } else {
    insQ.addBindValue(QVariant(QVariant::String));
  }

  if (!insQ.exec()) {
    insQ.finish();
    m_db.rollback();
    return execQuery(insQ, "insert updated action", error);
  }
  insQ.finish();

  // Commit with enhanced error checking
  if (!m_db.commit()) {
    if (error)
      *error = QString("Commit failed: %1").arg(m_db.lastError().text());
    return false;
  }

  return true;
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

  // Ensure database is open
  if (!m_db.isOpen()) {
    if (error) *error = "Database is not open";
    return false;
  }

  // Check if action exists before attempting to delete
  QSqlQuery checkQ(m_db);
  checkQ.prepare(
      "SELECT COUNT(*) FROM MacroActions WHERE macro_id=? AND \"order\"=?");
  checkQ.addBindValue(macroId);
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

  if (!actionExists) {
    if (error)
      *error = QString("Action not found at order %1 for macro %2")
                   .arg(order)
                   .arg(macroId);
    return false;
  }

  // Start clean transaction
  if (m_db.driver()->hasFeature(QSqlDriver::Transactions)) {
    m_db.rollback();  // Clean any existing transaction
  }

  if (!m_db.transaction()) {
    if (error)
      *error = QString("Failed to start transaction: %1")
                   .arg(m_db.lastError().text());
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
    m_db.rollback();
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
    m_db.rollback();
    if (error) *error = shiftError;
    return false;
  }
  shiftQ.finish();

  // Commit transaction
  if (!m_db.commit()) {
    QString commitError = QString("Failed to commit delete transaction: %1")
                              .arg(m_db.lastError().text());
    if (error) *error = commitError;
    Logger::instance().mError("Delete action commit failed: " + commitError);
    return false;
  }

  Logger::instance().mInfo(
      QString("Successfully deleted action at order %1 from macro %2")
          .arg(order)
          .arg(macroId));
  return true;
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
bool MacroManager::swapActions(int macroId, int order1, int order2,
                               QString* error) {
  if (!m_db.transaction()) {
    if (error) *error = "Transaction failed";
    return false;
  }

  // Geçici order kullanarak swap yap (-1000 gibi)
  const int tempOrder = -1000;

  QSqlQuery q(m_db);

  // İlk action'ı geçici order'a taşı
  q.prepare(
      "UPDATE MacroActions SET \"order\"=? WHERE macro_id=? AND \"order\"=?");
  q.addBindValue(tempOrder);
  q.addBindValue(macroId);
  q.addBindValue(order1);
  if (!q.exec()) {
    m_db.rollback();
    return execQuery(q, "move to temp", error);
  }

  // İkinci action'ı birincinin yerine taşı
  q.prepare(
      "UPDATE MacroActions SET \"order\"=? WHERE macro_id=? AND \"order\"=?");
  q.addBindValue(order1);
  q.addBindValue(macroId);
  q.addBindValue(order2);
  if (!q.exec()) {
    m_db.rollback();
    return execQuery(q, "move second", error);
  }

  // Geçici action'ı ikincinin yerine taşı
  q.prepare(
      "UPDATE MacroActions SET \"order\"=? WHERE macro_id=? AND \"order\"=?");
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
#include <QSqlDriver>

bool MacroManager::updateAction(int macroId, int oldOrder,
                                const MacroAction& newAction, QString* error) {
  if (!validateAction(newAction, error)) return false;

  // Ensure database is open
  if (!m_db.isOpen()) {
    if (error) *error = "Database is not open";
    return false;
  }

  // Check if transaction is already active
  bool wasInTransaction =
      m_db.driver()->hasFeature(QSqlDriver::Transactions) && m_db.transaction();

  if (!wasInTransaction) {
    if (error) *error = "Failed to start transaction";
    return false;
  }

  // Delete old action
  QSqlQuery delQ(m_db);
  delQ.prepare("DELETE FROM MacroActions WHERE macro_id=? AND \"order\"=?");
  delQ.addBindValue(macroId);
  delQ.addBindValue(oldOrder);
  if (!delQ.exec()) {
    m_db.rollback();
    return execQuery(delQ, "delete old action", error);
  }

  // Insert new action
  QSqlQuery insQ(m_db);
  insQ.prepare(
      "INSERT INTO MacroActions(macro_id, \"order\", action_type, click_type, "
      "repeat, position, current_position,"
      " interval, hold_duration, hover_duration, click_count, mouse_button, "
      "key_name)"
      " VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)");

  insQ.addBindValue(newAction.macro_id);
  insQ.addBindValue(newAction.order);
  insQ.addBindValue(actionTypeToStr(newAction.action_type));
  insQ.addBindValue(clickTypeToStr(newAction.click_type));
  insQ.addBindValue(newAction.repeat);

  // Handle optional fields properly
  insQ.addBindValue(newAction.position.has_value()
                        ? QVariant(newAction.position.value())
                        : QVariant(QVariant::String));
  insQ.addBindValue(newAction.current_pos.has_value()
                        ? (newAction.current_pos.value() ? 1 : 0)
                        : QVariant(QVariant::Int));
  insQ.addBindValue(newAction.interval);
  insQ.addBindValue(newAction.hold_duration.has_value()
                        ? QVariant(newAction.hold_duration.value())
                        : QVariant(QVariant::Int));
  insQ.addBindValue(newAction.hover_duration.has_value()
                        ? QVariant(newAction.hover_duration.value())
                        : QVariant(QVariant::Int));
  insQ.addBindValue(newAction.click_count);

  // FIXED: Proper MouseButton handling - ensure NULL for KEYBOARD actions
  if (newAction.action_type == ActionType::KEYBOARD ||
      !newAction.mouse_button.has_value()) {
    insQ.addBindValue(QVariant(QVariant::String));  // NULL
  } else {
    insQ.addBindValue(mouseButtonToStr(newAction.mouse_button.value()));
  }

  insQ.addBindValue(newAction.key_name.has_value()
                        ? QVariant(newAction.key_name.value())
                        : QVariant(QVariant::String));

  if (!insQ.exec()) {
    m_db.rollback();
    return execQuery(insQ, "insert updated action", error);
  }

  // Commit with error checking
  if (!m_db.commit()) {
    m_db.rollback();
    if (error)
      *error = QString("Commit failed: %1").arg(m_db.lastError().text());
    return false;
  }

  return true;
}

bool MacroManager::setActionsForMacro(int macroId,
                                      const QVector<MacroAction>& actions,
                                      QString* error) {
  if (!m_db.isOpen()) {
    if (error) *error = "Database is not open";
    return false;
  }

  Logger::instance().mInfo(
      QString("setActionsForMacro called for macro %1 with %2 actions")
          .arg(macroId)
          .arg(actions.size()));

  if (m_db.driver()->hasFeature(QSqlDriver::Transactions)) {
    m_db.rollback();
  }

  QThread::msleep(1);

  for (int i = 0; i < actions.size(); ++i) {
    const auto& action = actions[i];
    MacroAction validationAction = action;
    validationAction.macro_id = macroId;

    Logger::instance().mInfo(QString("Action %1: type=%2, click=%3")
                                 .arg(i)
                                 .arg(actionTypeToStr(action.action_type))
                                 .arg(clickTypeToStr(action.click_type)));

    if (!validateAction(validationAction, error)) {
      Logger::instance().mError(QString("Validation failed for action %1: %2")
                                    .arg(i)
                                    .arg(error ? *error : "unknown"));
      return false;
    }
  }

  if (!m_db.transaction()) {
    if (error)
      *error =
          QString("Transaction begin failed: %1").arg(m_db.lastError().text());
    Logger::instance().mError("Transaction begin failed");
    return false;
  }

  // Önce tüm eski action'ları sil
  QSqlQuery delQ(m_db);
  delQ.prepare("DELETE FROM MacroActions WHERE macro_id=?");
  delQ.addBindValue(macroId);
  if (!delQ.exec()) {
    m_db.rollback();
    Logger::instance().mError(
        QString("Delete failed: %1").arg(delQ.lastError().text()));
    return execQuery(delQ, "delete old actions", error);
  }

  Logger::instance().mInfo(QString("Deleted old actions, affected rows: %1")
                               .arg(delQ.numRowsAffected()));

  // Yeni action'ları ekle
  for (int i = 0; i < actions.size(); ++i) {
    const auto& action = actions[i];

    QSqlQuery insQ(m_db);
    insQ.prepare(
        "INSERT INTO MacroActions(macro_id, \"order\", action_type, "
        "click_type, "
        "repeat, position, current_position, interval, hold_duration, "
        "hover_duration, "
        "click_count, mouse_button, key_name) "
        "VALUES(?,?,?,?,?,?,?,?,?,?,?,?,?)");

    insQ.addBindValue(macroId);
    insQ.addBindValue(action.order);
    insQ.addBindValue(actionTypeToStr(action.action_type));
    insQ.addBindValue(clickTypeToStr(action.click_type));
    insQ.addBindValue(action.repeat);

    if (action.position.has_value()) {
      insQ.addBindValue(action.position.value());
    } else {
      insQ.addBindValue(QVariant(QVariant::String));
    }

    if (action.current_pos.has_value()) {
      insQ.addBindValue(action.current_pos.value() ? 1 : 0);
    } else {
      insQ.addBindValue(QVariant(QVariant::Int));
    }

    insQ.addBindValue(action.interval);

    if (action.hold_duration.has_value()) {
      insQ.addBindValue(action.hold_duration.value());
    } else {
      insQ.addBindValue(QVariant(QVariant::Int));
    }

    if (action.hover_duration.has_value()) {
      insQ.addBindValue(action.hover_duration.value());
    } else {
      insQ.addBindValue(QVariant(QVariant::Int));
    }

    insQ.addBindValue(action.click_count);

    if (action.action_type == ActionType::KEYBOARD ||
        !action.mouse_button.has_value()) {
      insQ.addBindValue(QVariant(QVariant::String));
    } else {
      insQ.addBindValue(mouseButtonToStr(action.mouse_button.value()));
    }

    if (action.key_name.has_value()) {
      insQ.addBindValue(action.key_name.value());
    } else {
      insQ.addBindValue(QVariant(QVariant::String));
    }

    if (!insQ.exec()) {
      m_db.rollback();
      Logger::instance().mError(QString("Insert failed for action %1: %2")
                                    .arg(i)
                                    .arg(insQ.lastError().text()));
      return execQuery(insQ, "insert action", error);
    }

    Logger::instance().mInfo(QString("Inserted action %1: type=%2, click=%3")
                                 .arg(i)
                                 .arg(actionTypeToStr(action.action_type))
                                 .arg(clickTypeToStr(action.click_type)));
  }

  if (!m_db.commit()) {
    if (error)
      *error = QString("Commit failed: %1").arg(m_db.lastError().text());
    Logger::instance().mError(
        QString("Commit failed: %1").arg(m_db.lastError().text()));
    return false;
  }

  Logger::instance().mInfo("Transaction committed successfully");
  return true;
}
bool MacroManager::normalizeOrders(int macroId, QString* error) {
  auto acts = getActions(macroId);
  if (acts.isEmpty()) return true;
  for (int i = 0; i < acts.size(); ++i) acts[i].order = i;
  return setActionsForMacro(macroId, acts, error);
}
