#ifndef ENUMS_H
#define ENUMS_H

#include <QString>
#include <optional>

enum class ActionType { MOUSE, KEYBOARD };
enum class ClickType { CLICK, HOLD, HOVER };
enum class MouseButton { LEFT, RIGHT, MID };
enum class Language { ENGLISH, TURKISH, GERMAN, FRENCH, ITALIAN };

// Enum → QString
inline QString actionTypeToStr(ActionType t) {
  switch (t) {
    case ActionType::MOUSE:
      return "mouse";
    case ActionType::KEYBOARD:
      return "keyboard";
  }
  return {};
}

inline QString clickTypeToStr(ClickType ctype) {
  switch (ctype) {
    case ClickType::CLICK:
      return "CLICK";
    case ClickType::HOLD:
      return "HOLD";
    case ClickType::HOVER:
      return "HOVER";
  }
  return "";
}

inline QString mouseButtonToStr(MouseButton b) {
  switch (b) {
    case MouseButton::LEFT:
      return "LEFT";
    case MouseButton::RIGHT:
      return "RIGHT";
    case MouseButton::MID:
      return "MID";
  }
  return {};
}

// QString → Enum
inline std::optional<ActionType> strToActionType(const QString& str) {
  QString s = str.trimmed().toLower();
  if (s == "mouse") return ActionType::MOUSE;
  if (s == "keyboard") return ActionType::KEYBOARD;
  return std::nullopt;
}

inline std::optional<ClickType> strToClickType(const QString& str) {
  QString s = str.trimmed().toUpper();
  if (s == "CLICK") return ClickType::CLICK;
  if (s == "HOLD") return ClickType::HOLD;
  if (s == "HOVER") return ClickType::HOVER;
  return std::nullopt;
}

inline std::optional<MouseButton> strToMouseButton(const QString& str) {
  QString s = str.trimmed().toUpper();
  if (s == "LEFT") return MouseButton::LEFT;
  if (s == "RIGHT") return MouseButton::RIGHT;
  if (s == "MID") return MouseButton::MID;
  return std::nullopt;
}

#endif  // ENUMS_H
