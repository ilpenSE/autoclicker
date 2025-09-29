// macromanager.h - Enhanced Validation System Additions
#pragma once
#include <QHash>
#include <QObject>
#include <QSet>
#include <QSqlDatabase>
#include <QString>
#include <QVector>
#include <optional>

#include "Enums.h"

struct Macro {
  int id = -1;
  QString name;
  QString description;
  QString hotkey;  // "DEF" | "F6" | "Shift + F6" ...
};

struct MacroAction {
  int macro_id = -1;
  int order = 0;
  ActionType action_type;                  // "mouse" | "keyboard"
  ClickType click_type;                    // "CLICK" | "HOLD" | "HOVER"
  int repeat = 1;                          // 0 = infinite
  std::optional<QString> position;         // "x, y"
  std::optional<bool> current_pos = true;  // uses current cursor pos
  int interval = 0;                        // ms
  std::optional<int> hold_duration;        // ms
  int click_count = 1;
  std::optional<MouseButton> mouse_button;  // "LEFT" | "RIGHT" | "MID" | ...
  std::optional<QString> key_name;
};

// helper
template <typename T>
static inline bool optEq(const std::optional<T>& a, const std::optional<T>& b) {
  if (a.has_value() != b.has_value()) return false;
  return !a.has_value() || a.value() == b.value();
}

inline bool operator==(const MacroAction& lhs, const MacroAction& rhs) {
  return lhs.order == rhs.order && lhs.action_type == rhs.action_type &&
         lhs.click_type == rhs.click_type &&
         optEq(lhs.mouse_button, rhs.mouse_button) &&
         lhs.click_count == rhs.click_count &&
         optEq(lhs.current_pos, rhs.current_pos) &&
         optEq(lhs.position, rhs.position) &&
         optEq(lhs.hold_duration, rhs.hold_duration) &&
         lhs.interval == rhs.interval && lhs.repeat == rhs.repeat &&
         optEq(lhs.key_name, rhs.key_name) && lhs.macro_id == rhs.macro_id;
}

inline bool operator!=(const MacroAction& lhs, const MacroAction& rhs) {
  return !(lhs == rhs);
}

class MacroManager final : public QObject {
  Q_OBJECT
 public:
  explicit MacroManager(QObject* parent = nullptr) : QObject(parent) {};

  static MacroManager& instance() {
    static MacroManager inst;
    return inst;
  }

  // init + schema
  bool init();
  bool isOpen() const;

  // Macros
  QVector<Macro> getAllMacros() const;
  bool existsMacro(int id);
  bool existsMacro(const QString& name);
  std::optional<Macro> getMacroById(int id) const;
  std::optional<Macro> getMacroByName(const QString& name) const;
  int createMacro(const Macro& m, QString* error = nullptr);
  bool updateMacro(const Macro& m, QString* error = nullptr);
  bool deleteMacro(int id, QString* error = nullptr);
  bool updateMacroName(int macroId, const QString& newName,
                       QString* error = nullptr);
  bool updateMacroDescription(int macroId, const QString& newDescription,
                              QString* error = nullptr);
  bool updateMacroHotkey(int macroId, const QString& newHotkey,
                         QString* error = nullptr);

  // Actions
  QVector<MacroAction> getActions(int macroId) const;
  bool existsAction(int macro_id, int order, QString* error = nullptr);
  bool addAction(const MacroAction& a, QString* error = nullptr);
  bool updateAction(int macroId, int oldOrder, const MacroAction& newAction,
                    QString* error);
  bool deleteAction(int macroId, int order, QString* error = nullptr);
  bool setActionsForMacro(int macroId, const QVector<MacroAction>& actions,
                          QString* error = nullptr);

  bool normalizeOrders(int macroId, QString* error = nullptr);

  bool updateOrder(int macroId, int oldOrder, int newOrder,
                                 const char* step, QString* error);
  bool moveActionUp(int macroId, int order, QString* error = nullptr);
  bool moveActionDown(int macroId, int order, QString* error = nullptr);
  bool swapActions(int macroId, int order1, int order2,
                   QString* error = nullptr);

  // Basic Validation
  bool validateMacroName(const QString& name, QString* error = nullptr) const;
  bool validateMacroDescription(const QString& desc,
                                QString* error = nullptr) const;
  bool validateHotkey(const QString& hotkey, QString* error = nullptr) const;
  bool validateAction(const MacroAction& a, QString* error = nullptr) const;

  // Path helper
  QString dbPath() const;

  // defaults
  MacroAction defaultAction = {
    1, 0, ActionType::MOUSE, ClickType::CLICK, 1, "0, 0", true, 100, 1000, 1, MouseButton::LEFT, "A"
  };
  Macro defaultMacro = {
    1, "DEFAULT", "The defaults.", "DEF"
  };

 private:
  MacroManager(const MacroManager&) = delete;
  MacroManager& operator=(const MacroManager&) = delete;

  bool ensureSchema(QString* error = nullptr);
  bool ensureDefaultMacro(QString* error = nullptr);
  bool execQuery(QSqlQuery& q, const char* ctx, QString* error) const;

  bool validateAndCreateMacrosTable(QString* error = nullptr);
  bool validateAndCreateActionsTable(QString* error = nullptr);
  bool recreateMacrosTable(QString* error = nullptr);
  bool recreateActionsTable(QString* error = nullptr);

  mutable QMap<QString, std::unique_ptr<QSqlQuery>> m_preparedQueries;

  const int tempOrder = -1000;

  QSqlDatabase m_db;

};
