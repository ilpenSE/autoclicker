#include "clickengine.h"
#include "macromanager.h"
#include "logger.h"
#include "LoggerStream.h"

#include <QApplication>
#include <QThread>
#include <QDebug>
#include <QCursor>
#include <QRegularExpression>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

// Singleton implementation
std::unique_ptr<ClickEngine> ClickEngine::m_instance = nullptr;
bool ClickEngine::m_destroyed = false;

ClickEngine* ClickEngine::getInstance()
{
    if (!m_instance && !m_destroyed) {
        m_instance = std::unique_ptr<ClickEngine>(new ClickEngine());
    }
    return m_instance.get();
}

void ClickEngine::destroyInstance()
{
    if (m_instance) {
        m_instance->stopCurrentMacro();
        m_instance.reset();
        m_destroyed = true;
    }
}

ClickEngine::ClickEngine(QObject* parent)
    : QObject(parent)
    , m_currentMacroId(-1)
    , m_executionTimer(nullptr)
{
    m_executionTimer = new QTimer(this);
    m_executionTimer->setSingleShot(true);
    connect(m_executionTimer, &QTimer::timeout, this, &ClickEngine::onExecutionTimer);

    cinfo() << "ClickEngine initialized";
}

ClickEngine::~ClickEngine()
{
    cleanup();
    cinfo() << "ClickEngine destroyed";
}

bool ClickEngine::startMacro(int macroId)
{
    // Stop current macro if any is running
    if (isMacroRunning()) {
        cinfo() << "Stopping current macro" << m_currentMacroId << "to start macro" << macroId;
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

    cinfo() << "Macro " << macroId << " started with " << actions.size() << " actions";
    return true;
}

bool ClickEngine::stopCurrentMacro()
{
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

bool ClickEngine::pauseCurrentMacro()
{
    if (!isMacroRunning()) {
        return false;
    }

    m_executionTimer->stop();
    m_executionState.isRunning = false;

    emit macroPaused(m_currentMacroId);
    cinfo() << "Macro " << m_currentMacroId << " paused";
    return true;
}

bool ClickEngine::resumeCurrentMacro()
{
    if (m_currentMacroId == -1 || m_executionState.isRunning) {
        return false;
    }

    m_executionState.isRunning = true;
    scheduleNextAction();

    emit macroResumed(m_currentMacroId);
    cinfo() << "Macro" << m_currentMacroId << "resumed";
    return true;
}

bool ClickEngine::isMacroRunning() const
{
    return m_currentMacroId != -1 && m_executionState.isRunning;
}

int ClickEngine::getCurrentMacroId() const
{
    return m_currentMacroId;
}

ClickEngine::ExecutionState ClickEngine::getCurrentMacroState() const
{
    return m_executionState;
}

void ClickEngine::executeMacroStep()
{
    if (m_stopRequested || !isMacroRunning() || m_currentActions.isEmpty()) {
        return;
    }

    // Check if we've reached the end of actions - loop back to start
    if (m_executionState.currentActionIndex >= m_currentActions.size()) {
        completeCurrentCycle();
        return;
    }

    const MacroAction& currentAction = m_currentActions[m_executionState.currentActionIndex];

    // Check if we need to repeat this action
    if (currentAction.repeat == 0 || m_executionState.currentRepeatCount < currentAction.repeat) {
        executeCurrentAction();

        // Increment repeat count (but not for infinite repeat)
        if (currentAction.repeat != 0) {
            m_executionState.currentRepeatCount++;
        }

        // Schedule next execution
        scheduleNextAction();
    } else {
        // Move to next action
        moveToNextAction();
    }
}

void ClickEngine::executeCurrentAction()
{
    if (m_stopRequested || !isMacroRunning() || m_currentActions.isEmpty()) {
        return;
    }

    const MacroAction& action = m_currentActions[m_executionState.currentActionIndex];

    try {
        performAction(action);
        m_executionState.totalActionsExecuted++;

        emit actionExecuted(m_currentMacroId, m_executionState.currentActionIndex,
                            m_executionState.totalActionsExecuted);

        cinfo() << "Executed action " << m_executionState.currentActionIndex
                 << " of macro " << m_currentMacroId << " (total:" << m_executionState.totalActionsExecuted << ")";
    } catch (...) {
        emit macroError(m_currentMacroId, QString("Error executing action %1").arg(m_executionState.currentActionIndex));
    }
}

void ClickEngine::scheduleNextAction()
{
    if (!isMacroRunning() || m_currentActions.isEmpty()) {
        return;
    }

    const MacroAction& currentAction = m_currentActions[m_executionState.currentActionIndex];
    int delay = currentAction.interval;

    // Minimum delay to prevent system overload
    if (delay < 10) {
        delay = 10;
    }

    m_executionTimer->start(delay);
}

void ClickEngine::moveToNextAction()
{
    m_executionState.currentActionIndex++;
    m_executionState.currentRepeatCount = 0;

    // Immediately execute next step (no additional delay between different actions)
    executeMacroStep();
}

void ClickEngine::completeCurrentCycle()
{
    m_executionState.currentActionIndex = 0;
    m_executionState.currentRepeatCount = 0;
    m_executionState.currentCycleCount++;

    emit cycleCompleted(m_currentMacroId, m_executionState.currentCycleCount);
    cinfo() << "Macro " << m_currentMacroId << " completed cycle " << m_executionState.currentCycleCount;

    // Continue with next cycle
    executeMacroStep();
}

void ClickEngine::performAction(const MacroAction& action)
{
    switch (action.action_type) {
    case ActionType::MOUSE:
        performMouseAction(action);
        break;
    case ActionType::KEYBOARD:
        performKeyboardAction(action);
        break;
    }
}

void ClickEngine::performMouseAction(const MacroAction& action)
{
    QPoint targetPos;

    if (action.current_pos) {
        targetPos = getCurrentMousePosition();
    } else if (action.position.has_value()) {
        targetPos = parsePosition(action.position.value());
    } else {
        targetPos = QPoint(0, 0);
    }

    MouseButton button = action.mouse_button.value_or(MouseButton::LEFT);

    switch (action.click_type) {
    case ClickType::CLICK:
        nativeMouseClick(targetPos, button, action.click_count);
        break;
    case ClickType::HOLD:
        nativeMouseHold(targetPos, button, action.hold_duration.value_or(1000));
        break;
    case ClickType::HOVER:
        nativeMouseHover(targetPos, action.hover_duration.value_or(1000));
        break;
    }
}

void ClickEngine::performKeyboardAction(const MacroAction& action)
{
    QString key = action.key_name;

    switch (action.click_type) {
    case ClickType::CLICK:
        nativeKeyClick(key, action.click_count);
        break;
    case ClickType::HOLD:
        nativeKeyHold(key, action.hold_duration.value_or(1000));
        break;
    case ClickType::HOVER:
        // Hover doesn't make sense for keyboard, treat as click
        nativeKeyClick(key, action.click_count);
        break;
    }
}

QPoint ClickEngine::getCurrentMousePosition() const
{
    return QCursor::pos();
}

void ClickEngine::setMousePosition(const QPoint& position)
{
    QCursor::setPos(position);
}

QPoint ClickEngine::parsePosition(const QString& positionStr)
{
    QRegularExpression rx(R"((\d+)\s*,\s*(\d+))");
    QRegularExpressionMatch match = rx.match(positionStr.trimmed());

    if (match.hasMatch()) {
        int x = match.captured(1).toInt();
        int y = match.captured(2).toInt();
        return QPoint(x, y);
    }
    return QPoint(0, 0);
}

QString ClickEngine::formatPosition(const QPoint& position)
{
    return QString("%1, %2").arg(position.x()).arg(position.y());
}

QList<MacroAction> ClickEngine::loadMacroActions(int macroId)
{
    QList<MacroAction> actions;
    QVector<MacroAction> dbActions = MacroManager::instance().getActions(macroId);

    for (const auto& action : dbActions) {
        actions.append(action);
    }

    return actions;
}

void ClickEngine::onExecutionTimer()
{
    executeMacroStep();
}

#ifdef Q_OS_WIN

void ClickEngine::nativeMouseClick(const QPoint& pos, MouseButton button, int count)
{
    if (m_stopRequested) return;

    if (count <= 0) return;

    nativeMousePress(pos, button);
    nativeMouseRelease(pos, button);

    if (count > 1) {
        // Geri kalan clickleri schedule et
        QTimer::singleShot(10, this, [=]() {
            nativeMouseClick(pos, button, count - 1);
        });
    }
}

void ClickEngine::nativeMousePress(const QPoint& pos, MouseButton button)
{
    SetCursorPos(pos.x(), pos.y());

    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dx = pos.x();
    input.mi.dy = pos.y();
    input.mi.dwFlags = mouseButtonToWin32(button, true) | MOUSEEVENTF_ABSOLUTE;

    SendInput(1, &input, sizeof(INPUT));
}

void ClickEngine::nativeMouseRelease(const QPoint& pos, MouseButton button)
{
    INPUT input = {};
    input.type = INPUT_MOUSE;
    input.mi.dx = pos.x();
    input.mi.dy = pos.y();
    input.mi.dwFlags = mouseButtonToWin32(button, false) | MOUSEEVENTF_ABSOLUTE;

    SendInput(1, &input, sizeof(INPUT));
}

void ClickEngine::nativeMouseMove(const QPoint& pos)
{
    SetCursorPos(pos.x(), pos.y());
}

void ClickEngine::nativeMouseHover(const QPoint& pos, int duration)
{
    if (m_stopRequested) return;

    nativeMouseMove(pos);

    QTimer::singleShot(duration, this, [=]() {
        if (!m_stopRequested) {
            // hover sadece bekleme demek, ekstra işlem gerekmez
        }
    });
}

void ClickEngine::nativeMouseHold(const QPoint& pos, MouseButton button, int duration)
{
    if (m_stopRequested) return;

    // 1. Press yap
    nativeMousePress(pos, button);

    // 2. duration kadar bekle
    QTimer::singleShot(duration, this, [=]() {
        if (!m_stopRequested) {
            // 3. Release yap
            nativeMouseRelease(pos, button);

            // 4. Hold bittikten sonra bir sonraki action'a geç
            moveToNextAction();
        }
    });
}


void ClickEngine::nativeKeyPress(const QString& key)
{
    WORD vk = qtKeyToVirtualKey(key);
    if (vk == 0) return;

    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    input.ki.dwFlags = 0;

    SendInput(1, &input, sizeof(INPUT));
}

void ClickEngine::nativeKeyRelease(const QString& key)
{
    WORD vk = qtKeyToVirtualKey(key);
    if (vk == 0) return;

    INPUT input = {};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = vk;
    input.ki.dwFlags = KEYEVENTF_KEYUP;

    SendInput(1, &input, sizeof(INPUT));
}

void ClickEngine::nativeKeyClick(const QString& key, int count)
{
    if (m_stopRequested) return;

    if (count <= 0) return;

    nativeKeyPress(key);
    nativeKeyRelease(key);

    if (count > 1) {
        QTimer::singleShot(10, this, [=]() {
            nativeKeyClick(key, count - 1);
        });
    }
}
void ClickEngine::nativeKeyHold(const QString& key, int duration)
{
    if (m_stopRequested) return;

    nativeKeyPress(key);

    QTimer::singleShot(duration, this, [=]() {
        if (!m_stopRequested) {
            nativeKeyRelease(key);
            moveToNextAction();
        }
    });
}

DWORD ClickEngine::mouseButtonToWin32(MouseButton button, bool isPress) const
{
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

WORD ClickEngine::qtKeyToVirtualKey(const QString& key) const
{
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
        {"SPACE", VK_SPACE}, {"TAB", VK_TAB}, {"ESCAPE", VK_ESCAPE}, {"ESC", VK_ESCAPE},
        {"BACKSPACE", VK_BACK}, {"DELETE", VK_DELETE}, {"INSERT", VK_INSERT},
        {"HOME", VK_HOME}, {"END", VK_END}, {"PAGEUP", VK_PRIOR}, {"PAGEDOWN", VK_NEXT},
        {"UP", VK_UP}, {"DOWN", VK_DOWN}, {"LEFT", VK_LEFT}, {"RIGHT", VK_RIGHT},
        {"SHIFT", VK_SHIFT}, {"CTRL", VK_CONTROL}, {"ALT", VK_MENU},
        {"LSHIFT", VK_LSHIFT}, {"RSHIFT", VK_RSHIFT},
        {"LCTRL", VK_LCONTROL}, {"RCTRL", VK_RCONTROL},
        {"LALT", VK_LMENU}, {"RALT", VK_RMENU},
        {"CAPSLOCK", VK_CAPITAL}, {"NUMLOCK", VK_NUMLOCK}, {"SCROLLLOCK", VK_SCROLL},
        {"PRINTSCREEN", VK_SNAPSHOT}, {"PAUSE", VK_PAUSE},
        // Numpad keys
        {"NUMPAD0", VK_NUMPAD0}, {"NUMPAD1", VK_NUMPAD1}, {"NUMPAD2", VK_NUMPAD2},
        {"NUMPAD3", VK_NUMPAD3}, {"NUMPAD4", VK_NUMPAD4}, {"NUMPAD5", VK_NUMPAD5},
        {"NUMPAD6", VK_NUMPAD6}, {"NUMPAD7", VK_NUMPAD7}, {"NUMPAD8", VK_NUMPAD8},
        {"NUMPAD9", VK_NUMPAD9}, {"NUMPADDOT", VK_DECIMAL}, {"NUMPADPLUS", VK_ADD},
        {"NUMPADMINUS", VK_SUBTRACT}, {"NUMPADMULTIPLY", VK_MULTIPLY}, {"NUMPADDIVIDE", VK_DIVIDE},
        // Common punctuation
        {"SEMICOLON", VK_OEM_1}, {"EQUALS", VK_OEM_PLUS}, {"COMMA", VK_OEM_COMMA},
        {"MINUS", VK_OEM_MINUS}, {"PERIOD", VK_OEM_PERIOD}, {"SLASH", VK_OEM_2},
        {"BACKTICK", VK_OEM_3}, {"LBRACKET", VK_OEM_4}, {"BACKSLASH", VK_OEM_5},
        {"RBRACKET", VK_OEM_6}, {"QUOTE", VK_OEM_7}
    };

    if (specialKeys.contains(upperKey)) {
        return specialKeys[upperKey];
    }

    return 0; // Unknown key
}

#else
// Linux/macOS implementation stubs
void ClickEngine::nativeMouseClick(const QPoint& pos, MouseButton button, int count) { Q_UNUSED(pos) Q_UNUSED(button) Q_UNUSED(count) }
void ClickEngine::nativeMousePress(const QPoint& pos, MouseButton button) { Q_UNUSED(pos) Q_UNUSED(button) }
void ClickEngine::nativeMouseRelease(const QPoint& pos, MouseButton button) { Q_UNUSED(pos) Q_UNUSED(button) }
void ClickEngine::nativeMouseMove(const QPoint& pos) { Q_UNUSED(pos) }
void ClickEngine::nativeMouseHover(const QPoint& pos, int duration) { Q_UNUSED(pos) Q_UNUSED(duration) }
void ClickEngine::nativeMouseHold(const QPoint& pos, MouseButton button, int duration) { Q_UNUSED(pos) Q_UNUSED(button) Q_UNUSED(duration) }
void ClickEngine::nativeKeyPress(const QString& key) { Q_UNUSED(key) }
void ClickEngine::nativeKeyRelease(const QString& key) { Q_UNUSED(key) }
void ClickEngine::nativeKeyClick(const QString& key, int count) { Q_UNUSED(key) Q_UNUSED(count) }
void ClickEngine::nativeKeyHold(const QString& key, int duration) { Q_UNUSED(key) Q_UNUSED(duration) }
#endif

QString ClickEngine::mouseButtonToString(MouseButton button) const
{
    return mouseButtonToStr(button);
}

MouseButton ClickEngine::stringToMouseButton(const QString& buttonStr) const
{
    auto result = strToMouseButton(buttonStr);
    return result.value_or(MouseButton::LEFT);
}

void ClickEngine::cleanup()
{
    stopCurrentMacro();

    if (m_executionTimer) {
        m_executionTimer->deleteLater();
        m_executionTimer = nullptr;
    }
}
