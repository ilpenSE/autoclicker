#include "clickengine.h"

#include <QApplication>
#include <QCursor>
#include <QDebug>
#include <QRegularExpression>
#include <QThread>

#include "LoggerStream.h"
#include "logger.h"
#include "macromanager.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

// Singleton implementation
std::unique_ptr<ClickEngine> ClickEngine::m_instance = nullptr;
bool ClickEngine::m_destroyed = false;

ClickEngine* ClickEngine::getInstance() {
  if (!m_instance && !m_destroyed) {
    m_instance = std::unique_ptr<ClickEngine>(new ClickEngine());
  }
  return m_instance.get();
}

void ClickEngine::destroyInstance() {
  if (m_instance) {
    m_instance->stopCurrentMacro();
    m_instance.reset();
    m_destroyed = true;
  }
}

ClickEngine::ClickEngine(QObject* parent)
    : QObject(parent), m_currentMacroId(-1), m_executionTimer(nullptr) {
  m_executionTimer = new QTimer(this);
  m_executionTimer->setSingleShot(true);
  connect(m_executionTimer, &QTimer::timeout, this,
          &ClickEngine::onExecutionTimer);

  cinfo() << "ClickEngine initialized";
}

ClickEngine::~ClickEngine() {
  cleanup();
  cinfo() << "ClickEngine destroyed";
}

bool ClickEngine::startMacro(int macroId) {
  // Stop current macro if any is running
  if (isMacroRunning()) {
    cinfo() << "Stopping current macro" << m_currentMacroId << "to start macro"
            << macroId;
    stopCurrentMacro();
  }

  m_stopRequested = false;

  // Load macro actions from database
  QList<MacroAction> actions = loadMacroActions(macroId);
  if (actions.isEmpty()) {
    emit macroError(macroId, "No actions found for macro");
    return false;
  }

  // Initialize execution state
  m_currentMacroId = macroId;
  m_currentActions = actions;
  m_executionState = ExecutionState();
  m_executionState.isRunning = true;

  // Start execution immediately
  executeMacroStep();
  emit macroStarted(macroId);

  cinfo() << "Macro " << macroId << " started with " << actions.size()
          << " actions";
  return true;
}

bool ClickEngine::stopCurrentMacro() {
  if (!isMacroRunning()) {
    return false;
  }

  m_stopRequested = true;

  int stoppedMacroId = m_currentMacroId;

  m_executionTimer->stop();
  m_currentMacroId = -1;
  m_executionState = ExecutionState();
  m_currentActions.clear();

  emit macroStopped(stoppedMacroId);
  cinfo() << "Macro " << stoppedMacroId << " stopped";
  return true;
}

bool ClickEngine::pauseCurrentMacro() {
  if (!isMacroRunning()) {
    return false;
  }

  m_executionTimer->stop();
  m_executionState.isRunning = false;

  emit macroPaused(m_currentMacroId);
  cinfo() << "Macro " << m_currentMacroId << " paused";
  return true;
}

bool ClickEngine::resumeCurrentMacro() {
  if (m_currentMacroId == -1 || m_executionState.isRunning) {
    return false;
  }

  m_executionState.isRunning = true;
  scheduleNextAction();

  emit macroResumed(m_currentMacroId);
  cinfo() << "Macro" << m_currentMacroId << "resumed";
  return true;
}

bool ClickEngine::isMacroRunning() const {
  return m_currentMacroId != -1 && m_executionState.isRunning;
}

int ClickEngine::getCurrentMacroId() const { return m_currentMacroId; }

ClickEngine::ExecutionState ClickEngine::getCurrentMacroState() const {
  return m_executionState;
}

void ClickEngine::executeMacroStep() {
  if (m_stopRequested || !isMacroRunning() || m_currentActions.isEmpty()) {
    return;
  }

  if (m_executionState.currentActionIndex >= m_currentActions.size()) {
    stopCurrentMacro();  // completeCurrentCycle() yerine direkt durdur
    return;
  }

  const MacroAction& currentAction =
      m_currentActions[m_executionState.currentActionIndex];

  executeCurrentAction();
  m_executionState.currentRepeatCount++;

  // Repeat kontrolü
  if (currentAction.repeat == 0 ||
      m_executionState.currentRepeatCount < currentAction.repeat) {
    // Daha fazla repeat gerekiyor, interval bekle
    scheduleNextRepeat();
  } else {
    // Bu action bitti, sonrakine geç
    moveToNextAction();
  }
}

void ClickEngine::scheduleNextRepeat() {
  if (!isMacroRunning() || m_currentActions.isEmpty()) {
    return;
  }

  const MacroAction& currentAction =
      m_currentActions[m_executionState.currentActionIndex];
  int delay = currentAction.interval;

  // Minimum delay to prevent system overload
  if (delay < 10) {
    delay = 10;
  }

  // Use timer to wait for interval before next repeat
  m_executionTimer->start(delay);
}

void ClickEngine::executeCurrentAction() {
  if (m_stopRequested || !isMacroRunning() || m_currentActions.isEmpty()) {
    return;
  }

  const MacroAction& action =
      m_currentActions[m_executionState.currentActionIndex];

  try {
    performAction(action);
    m_executionState.totalActionsExecuted++;

    emit actionExecuted(m_currentMacroId, m_executionState.currentActionIndex,
                        m_executionState.totalActionsExecuted);

    cinfo() << "Executed action " << m_executionState.currentActionIndex
            << " of macro " << m_currentMacroId
            << " (total:" << m_executionState.totalActionsExecuted << ")";
  } catch (...) {
    emit macroError(m_currentMacroId,
                    QString("Error executing action %1")
                        .arg(m_executionState.currentActionIndex));
  }
}

void ClickEngine::scheduleNextAction() {
  if (!isMacroRunning() || m_currentActions.isEmpty()) {
    return;
  }

  const MacroAction& currentAction =
      m_currentActions[m_executionState.currentActionIndex];
  int delay = currentAction.interval;

  // Minimum delay to prevent system overload
  if (delay < 10) {
    delay = 10;
  }

  m_executionTimer->start(delay);
}

void ClickEngine::moveToNextAction() {
  m_executionState.currentActionIndex++;
  m_executionState.currentRepeatCount = 0;  // ÖNEMLİ: reset et

  executeMacroStep();  // Direkt devam et
}

void ClickEngine::completeCurrentCycle() {
  m_executionState.currentActionIndex = 0;
  m_executionState.currentRepeatCount = 0;
  m_executionState.currentCycleCount++;

  emit cycleCompleted(m_currentMacroId, m_executionState.currentCycleCount);
  cinfo() << "Macro " << m_currentMacroId << " completed cycle "
          << m_executionState.currentCycleCount;

  // Continue with next cycle
  executeMacroStep();
}

void ClickEngine::performAction(const MacroAction& action) {
  switch (action.action_type) {
    case ActionType::MOUSE:
      performMouseAction(action);
      break;
    case ActionType::KEYBOARD:
      performKeyboardAction(action);
      break;
  }
}

void ClickEngine::performMouseAction(const MacroAction& action) {
  QPoint targetPos;

  if (action.current_pos.value_or(true)) {
    targetPos = getCurrentMousePosition();
  } else if (action.position.has_value()) {
    targetPos = parsePosition(action.position.value());
  } else {
    targetPos = QPoint(0, 0);
  }

  MouseButton button = action.mouse_button.value_or(MouseButton::LEFT);

  switch (action.click_type) {
    case ClickType::CLICK:
      // Execute all click_count clicks immediately (no interval between them)
      nativeMouseClick(targetPos, button, action.click_count);
      break;

    case ClickType::HOLD:
      // Hold ignores click_count - just press, wait, release
      nativeMouseHold(targetPos, button, action.hold_duration.value_or(1000));
      break;

    case ClickType::HOVER:
      // Hover to position and wait
      nativeMouseHover(targetPos, 10);
      break;
  }
}

void ClickEngine::performKeyboardAction(const MacroAction& action) {
  QString key = action.key_name.value_or("A");

  switch (action.click_type) {
    case ClickType::CLICK:
      // Execute all click_count key presses immediately
      nativeKeyClick(key, action.click_count);
      break;

    case ClickType::HOLD:
      nativeKeyHold(action.key_name.value(), action.hold_duration.value());
      break;

    case ClickType::HOVER:
      // Hover doesn't make sense for keyboard, treat as single click
      nativeKeyClick(key, 1);
      break;
  }
}

QPoint ClickEngine::getCurrentMousePosition() const { return QCursor::pos(); }

void ClickEngine::setMousePosition(const QPoint& position) {
  QCursor::setPos(position);
}

QPoint ClickEngine::parsePosition(const QString& positionStr) {
  QRegularExpression rx(R"((\d+)\s*,\s*(\d+))");
  QRegularExpressionMatch match = rx.match(positionStr.trimmed());

  if (match.hasMatch()) {
    int x = match.captured(1).toInt();
    int y = match.captured(2).toInt();
    return QPoint(x, y);
  }
  return QPoint(0, 0);
}

QString ClickEngine::formatPosition(const QPoint& position) {
  return QString("%1, %2").arg(position.x()).arg(position.y());
}

QList<MacroAction> ClickEngine::loadMacroActions(int macroId) {
  QList<MacroAction> actions;
  QVector<MacroAction> dbActions = MacroManager::instance().getActions(macroId);

  for (const auto& action : dbActions) {
    actions.append(action);
  }

  return actions;
}

void ClickEngine::onExecutionTimer() { executeMacroStep(); }

#ifdef Q_OS_WIN
void ClickEngine::nativeMouseClick(const QPoint& pos, MouseButton button,
                                   int clickCount) {
  if (m_stopRequested || clickCount <= 0) return;

  SetCursorPos(pos.x(), pos.y());

  // Perform clickCount number of clicks with minimal delay between them
  for (int i = 0; i < clickCount; i++) {
    if (m_stopRequested) break;

    // Press
    INPUT inputDown = {};
    inputDown.type = INPUT_MOUSE;
    inputDown.mi.dx = pos.x();
    inputDown.mi.dy = pos.y();
    inputDown.mi.dwFlags =
        mouseButtonToWin32(button, true) | MOUSEEVENTF_ABSOLUTE;
    SendInput(1, &inputDown, sizeof(INPUT));

    // Small delay between press and release
    Sleep(1);

    // Release
    INPUT inputUp = {};
    inputUp.type = INPUT_MOUSE;
    inputUp.mi.dx = pos.x();
    inputUp.mi.dy = pos.y();
    inputUp.mi.dwFlags =
        mouseButtonToWin32(button, false) | MOUSEEVENTF_ABSOLUTE;
    SendInput(1, &inputUp, sizeof(INPUT));

    // Small delay between clicks within the same click group (except for last
    // one)
    if (i < clickCount - 1) {
      Sleep(50);  // 50ms between clicks in the same group
    }
  }
}

void ClickEngine::nativeMouseHold(const QPoint& pos, MouseButton button,
                                  int duration) {
  if (m_stopRequested) return;

  SetCursorPos(pos.x(), pos.y());

  // Press
  INPUT inputDown = {};
  inputDown.type = INPUT_MOUSE;
  inputDown.mi.dx = pos.x();
  inputDown.mi.dy = pos.y();
  inputDown.mi.dwFlags =
      mouseButtonToWin32(button, true) | MOUSEEVENTF_ABSOLUTE;
  SendInput(1, &inputDown, sizeof(INPUT));

  // Wait for hold duration using Windows Sleep to avoid Qt event loop
  // interference
  Sleep(duration);

  if (!m_stopRequested) {
    // Release
    INPUT inputUp = {};
    inputUp.type = INPUT_MOUSE;
    inputUp.mi.dx = pos.x();
    inputUp.mi.dy = pos.y();
    inputUp.mi.dwFlags =
        mouseButtonToWin32(button, false) | MOUSEEVENTF_ABSOLUTE;
    SendInput(1, &inputUp, sizeof(INPUT));
  }
}

void ClickEngine::nativeMouseHover(const QPoint& pos, int duration) {
  if (m_stopRequested) return;

  SetCursorPos(pos.x(), pos.y());

  // Simply wait at the position
  Sleep(duration);
}

void ClickEngine::nativeKeyClick(const QString& key, int clickCount) {
  if (m_stopRequested || clickCount <= 0) return;

  WORD vk = qtKeyToVirtualKey(key);
  if (vk == 0) return;

  // Perform clickCount number of key presses with minimal delay
  for (int i = 0; i < clickCount; i++) {
    if (m_stopRequested) break;

    // Press
    INPUT inputDown = {};
    inputDown.type = INPUT_KEYBOARD;
    inputDown.ki.wVk = vk;
    inputDown.ki.dwFlags = 0;
    SendInput(1, &inputDown, sizeof(INPUT));

    // Small delay between press and release
    Sleep(1);

    // Release
    INPUT inputUp = {};
    inputUp.type = INPUT_KEYBOARD;
    inputUp.ki.wVk = vk;
    inputUp.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &inputUp, sizeof(INPUT));

    // Small delay between key presses within the same group (except for last
    // one)
    if (i < clickCount - 1) {
      Sleep(50);  // 50ms between key presses in the same group
    }
  }
}
void ClickEngine::nativeKeyHold(const QString& keyCombo, int duration) {
  if (m_stopRequested) return;

  QStringList parts = keyCombo.split('+', Qt::SkipEmptyParts);
  QList<WORD> modifiers;
  WORD normalKey = 0;

  for (const QString& part : parts) {
    QString upper = part.trimmed().toUpper();
    if (upper == "SHIFT")
      modifiers.append(VK_SHIFT);
    else if (upper == "CTRL")
      modifiers.append(VK_CONTROL);
    else if (upper == "ALT")
      modifiers.append(VK_MENU);
    else
      normalKey = qtKeyToVirtualKey(upper);  // normal key
  }

  if (normalKey == 0) return;

  int totalDuration = duration;
  int pressInterval = 10;
  int repeatCount = totalDuration / pressInterval;

  for (int i = 0; i < repeatCount && !m_stopRequested; i++) {
    // Press modifiers
    for (WORD mod : modifiers) {
      INPUT modDown = {};
      modDown.type = INPUT_KEYBOARD;
      modDown.ki.wVk = mod;
      modDown.ki.dwFlags = 0;
      SendInput(1, &modDown, sizeof(INPUT));
    }

    // Press normal key
    INPUT keyDown = {};
    keyDown.type = INPUT_KEYBOARD;
    keyDown.ki.wVk = normalKey;
    keyDown.ki.dwFlags = 0;
    SendInput(1, &keyDown, sizeof(INPUT));

    Sleep(10);  // kısa basış

    // Release normal key
    INPUT keyUp = {};
    keyUp.type = INPUT_KEYBOARD;
    keyUp.ki.wVk = normalKey;
    keyUp.ki.dwFlags = KEYEVENTF_KEYUP;
    SendInput(1, &keyUp, sizeof(INPUT));

    // Release modifiers
    for (WORD mod : modifiers) {
      INPUT modUp = {};
      modUp.type = INPUT_KEYBOARD;
      modUp.ki.wVk = mod;
      modUp.ki.dwFlags = KEYEVENTF_KEYUP;
      SendInput(1, &modUp, sizeof(INPUT));
    }

    Sleep(pressInterval - 10);
  }
}

DWORD ClickEngine::mouseButtonToWin32(MouseButton button, bool isPress) const {
  switch (button) {
    case MouseButton::LEFT:
      return isPress ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
    case MouseButton::RIGHT:
      return isPress ? MOUSEEVENTF_RIGHTDOWN : MOUSEEVENTF_RIGHTUP;
    case MouseButton::MID:
      return isPress ? MOUSEEVENTF_MIDDLEDOWN : MOUSEEVENTF_MIDDLEUP;
    default:
      return isPress ? MOUSEEVENTF_LEFTDOWN : MOUSEEVENTF_LEFTUP;
  }
}

WORD ClickEngine::qtKeyToVirtualKey(const QString& key) const {
  QString upperKey = key.toUpper();

  // Single character letters and numbers
  if (upperKey.length() == 1) {
    QChar c = upperKey[0];
    if (c >= 'A' && c <= 'Z') {
      return c.toLatin1();
    }
    if (c >= '0' && c <= '9') {
      return c.toLatin1();
    }
  }

  // Function keys
  if (upperKey.startsWith("F")) {
    bool ok;
    int fNum = upperKey.mid(1).toInt(&ok);
    if (ok && fNum >= 1 && fNum <= 24) {
      return VK_F1 + (fNum - 1);
    }
  }

  // Special keys map
  static const QMap<QString, WORD> specialKeys = {
      {"SPACE", VK_SPACE},
      {"TAB", VK_TAB},
      {"ESCAPE", VK_ESCAPE},
      {"ESC", VK_ESCAPE},
      {"BACKSPACE", VK_BACK},
      {"DELETE", VK_DELETE},
      {"INSERT", VK_INSERT},
      {"HOME", VK_HOME},
      {"END", VK_END},
      {"PAGEUP", VK_PRIOR},
      {"PAGEDOWN", VK_NEXT},
      {"UP", VK_UP},
      {"DOWN", VK_DOWN},
      {"LEFT", VK_LEFT},
      {"RIGHT", VK_RIGHT},
      {"SHIFT", VK_SHIFT},
      {"CTRL", VK_CONTROL},
      {"ALT", VK_MENU},
      {"LSHIFT", VK_LSHIFT},
      {"RSHIFT", VK_RSHIFT},
      {"LCTRL", VK_LCONTROL},
      {"RCTRL", VK_RCONTROL},
      {"LALT", VK_LMENU},
      {"RALT", VK_RMENU},
      {"CAPSLOCK", VK_CAPITAL},
      {"NUMLOCK", VK_NUMLOCK},
      {"SCROLLLOCK", VK_SCROLL},
      {"PRINTSCREEN", VK_SNAPSHOT},
      {"PAUSE", VK_PAUSE},
      // Numpad keys
      {"NUMPAD0", VK_NUMPAD0},
      {"NUMPAD1", VK_NUMPAD1},
      {"NUMPAD2", VK_NUMPAD2},
      {"NUMPAD3", VK_NUMPAD3},
      {"NUMPAD4", VK_NUMPAD4},
      {"NUMPAD5", VK_NUMPAD5},
      {"NUMPAD6", VK_NUMPAD6},
      {"NUMPAD7", VK_NUMPAD7},
      {"NUMPAD8", VK_NUMPAD8},
      {"NUMPAD9", VK_NUMPAD9},
      {"NUMPADDOT", VK_DECIMAL},
      {"NUMPADPLUS", VK_ADD},
      {"NUMPADMINUS", VK_SUBTRACT},
      {"NUMPADMULTIPLY", VK_MULTIPLY},
      {"NUMPADDIVIDE", VK_DIVIDE},
      // Common punctuation
      {"SEMICOLON", VK_OEM_1},
      {"EQUALS", VK_OEM_PLUS},
      {"COMMA", VK_OEM_COMMA},
      {"MINUS", VK_OEM_MINUS},
      {"PERIOD", VK_OEM_PERIOD},
      {"SLASH", VK_OEM_2},
      {"BACKTICK", VK_OEM_3},
      {"LBRACKET", VK_OEM_4},
      {"BACKSLASH", VK_OEM_5},
      {"RBRACKET", VK_OEM_6},
      {"QUOTE", VK_OEM_7}};

  if (specialKeys.contains(upperKey)) {
    return specialKeys[upperKey];
  }

  return 0;  // Unknown key
}

#else
// Linux/macOS implementation stubs
void ClickEngine::nativeMouseClick(const QPoint& pos, MouseButton button,
                                   int count) {
  Q_UNUSED(pos)
  Q_UNUSED(button)
  Q_UNUSED(count)
}
void ClickEngine::nativeMousePress(const QPoint& pos, MouseButton button) {
  Q_UNUSED(pos)
  Q_UNUSED(button)
}
void ClickEngine::nativeMouseRelease(const QPoint& pos, MouseButton button) {
  Q_UNUSED(pos)
  Q_UNUSED(button)
}
void ClickEngine::nativeMouseMove(const QPoint& pos) { Q_UNUSED(pos) }
void ClickEngine::nativeMouseHover(const QPoint& pos, int duration) {
  Q_UNUSED(pos)
  Q_UNUSED(duration)
}
void ClickEngine::nativeMouseHold(const QPoint& pos, MouseButton button,
                                  int duration) {
  Q_UNUSED(pos)
  Q_UNUSED(button)
  Q_UNUSED(duration)
}
void ClickEngine::nativeKeyPress(const QString& key) { Q_UNUSED(key) }
void ClickEngine::nativeKeyRelease(const QString& key) { Q_UNUSED(key) }
void ClickEngine::nativeKeyClick(const QString& key, int count) {
  Q_UNUSED(key)
  Q_UNUSED(count)
}
void ClickEngine::nativeKeyHold(const QString& key, int duration) {
  Q_UNUSED(key)
  Q_UNUSED(duration)
}
#endif

QString ClickEngine::mouseButtonToString(MouseButton button) const {
  return mouseButtonToStr(button);
}

MouseButton ClickEngine::stringToMouseButton(const QString& buttonStr) const {
  auto result = strToMouseButton(buttonStr);
  return result.value_or(MouseButton::LEFT);
}

void ClickEngine::cleanup() {
  stopCurrentMacro();

  if (m_executionTimer) {
    m_executionTimer->deleteLater();
    m_executionTimer = nullptr;
  }
}
