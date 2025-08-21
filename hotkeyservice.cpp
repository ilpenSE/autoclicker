#include "hotkeyservice.h"
#include <QApplication>
#include <QDebug>
#include <QStringList>
#include <QRegularExpression>
#include "logger.h"
#include "LoggerStream.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif

// Singleton implementation
std::unique_ptr<HotkeyService> HotkeyService::m_instance = nullptr;
bool HotkeyService::m_destroyed = false;

HotkeyService* HotkeyService::getInstance()
{
    if (!m_instance && !m_destroyed) {
        m_instance = std::unique_ptr<HotkeyService>(new HotkeyService());
        qApp->installNativeEventFilter(m_instance.get());
    }
    return m_instance.get();
}

void HotkeyService::destroyInstance()
{
    if (m_instance) {
        qApp->removeNativeEventFilter(m_instance.get());
        m_instance.reset();
        m_destroyed = true;
    }
}

HotkeyService::HotkeyService(QObject* parent)
    : QObject(parent), m_nextHotkeyId(1000) // Start from 1000 to avoid conflicts
{
    hsinfo() << "Hotkey Service initialized";
}

HotkeyService::~HotkeyService()
{
    cleanup();
    hsinfo() << ("Hotkey Service destroyed");
}

bool HotkeyService::registerHotkey(const QString& hotkeyString, int id)
{
    if (hotkeyString.isEmpty() || id <= 0) {
        emit hotkeyRegistrationFailed(id, "Invalid parameters");
        return false;
    }

    if (m_registeredHotkeys.contains(id)) {
        emit hotkeyRegistrationFailed(id, "Hotkey ID already registered");
        return false;
    }

    if (!isValidHotkeyString(hotkeyString)) {
        emit hotkeyRegistrationFailed(id, "Invalid hotkey string format");
        return false;
    }

    Qt::KeyboardModifiers modifiers;
    int key;
    if (!parseHotkeyString(hotkeyString, modifiers, key)) {
        emit hotkeyRegistrationFailed(id, "Failed to parse hotkey string");
        return false;
    }

    if (!registerNativeHotkey(id, modifiers, key)) {
        emit hotkeyRegistrationFailed(id, "Failed to register native hotkey");
        return false;
    }

    HotkeyData hotkeyData;
    hotkeyData.id = id;
    hotkeyData.keySequence = hotkeyString;
    hotkeyData.modifiers = modifiers;
    hotkeyData.key = key;
    hotkeyData.isRegistered = true;

    m_registeredHotkeys.insert(id, hotkeyData);
    hsinfo() << "Hotkey registered successfully: " << hotkeyString << " with ID: " << id;
    return true;
}

bool HotkeyService::unregisterHotkey(int id)
{
    if (!m_registeredHotkeys.contains(id)) {
        return false;
    }

    if (!unregisterNativeHotkey(id)) {
        hserr() << "Failed to unregister native hotkey for ID:" << id;
        return false;
    }

    m_registeredHotkeys.remove(id);
    hsinfo() << "Hotkey unregistered successfully for ID:" << id;
    return true;
}

bool HotkeyService::changeHotkey(int id, const QString& newHotkeyString)
{
    if (!m_registeredHotkeys.contains(id)) {
        return false;
    }

    // Unregister current hotkey
    if (!unregisterHotkey(id)) {
        return false;
    }

    // Register new hotkey
    return registerHotkey(newHotkeyString, id);
}

void HotkeyService::unregisterAllHotkeys()
{
    QList<int> ids = m_registeredHotkeys.keys();
    for (int id : ids) {
        unregisterHotkey(id);
    }
}

bool HotkeyService::isHotkeyRegistered(int id) const
{
    return m_registeredHotkeys.contains(id) && m_registeredHotkeys[id].isRegistered;
}

QString HotkeyService::getHotkeyString(int id) const
{
    if (m_registeredHotkeys.contains(id)) {
        return m_registeredHotkeys[id].keySequence;
    }
    return QString();
}

QList<int> HotkeyService::getRegisteredHotkeyIds() const
{
    return m_registeredHotkeys.keys();
}

bool HotkeyService::parseHotkeyString(const QString& hotkeyString, Qt::KeyboardModifiers& modifiers, int& key)
{
    modifiers = Qt::NoModifier;
    key = Qt::Key_unknown;

    QStringList parts = hotkeyString.split(" + ", Qt::SkipEmptyParts);
    if (parts.isEmpty()) {
        return false;
    }

    // Son part key, öncekiler modifier
    QString keyStr = parts.last().toUpper();

    // Modifierları parse et
    for (int i = 0; i < parts.size() - 1; ++i) {
        QString modifier = parts[i].toLower();
        if (modifier == "ctrl") {
            modifiers |= Qt::ControlModifier;
        } else if (modifier == "shift") {
            modifiers |= Qt::ShiftModifier;
        } else if (modifier == "alt") {
            modifiers |= Qt::AltModifier;
        } else {
            return false; // Unknown modifier
        }
    }

    // Function keys
    if (keyStr.startsWith("F") && keyStr.length() > 1) {
        bool ok;
        int fNum = keyStr.mid(1).toInt(&ok);
        if (ok && fNum >= 1 && fNum <= 12) {
            key = Qt::Key_F1 + (fNum - 1);
            return true;
        }
    }

    // Normal keys
    QMap<QString, Qt::Key> keyMap = {
        {"A", Qt::Key_A}, {"B", Qt::Key_B}, {"C", Qt::Key_C}, {"D", Qt::Key_D},
        {"E", Qt::Key_E}, {"F", Qt::Key_F}, {"G", Qt::Key_G}, {"H", Qt::Key_H},
        {"I", Qt::Key_I}, {"J", Qt::Key_J}, {"K", Qt::Key_K}, {"L", Qt::Key_L},
        {"M", Qt::Key_M}, {"N", Qt::Key_N}, {"O", Qt::Key_O}, {"P", Qt::Key_P},
        {"Q", Qt::Key_Q}, {"R", Qt::Key_R}, {"S", Qt::Key_S}, {"T", Qt::Key_T},
        {"U", Qt::Key_U}, {"V", Qt::Key_V}, {"W", Qt::Key_W}, {"X", Qt::Key_X},
        {"Y", Qt::Key_Y}, {"Z", Qt::Key_Z},
        {"0", Qt::Key_0}, {"1", Qt::Key_1}, {"2", Qt::Key_2}, {"3", Qt::Key_3},
        {"4", Qt::Key_4}, {"5", Qt::Key_5}, {"6", Qt::Key_6}, {"7", Qt::Key_7},
        {"8", Qt::Key_8}, {"9", Qt::Key_9},
        {"SPACE", Qt::Key_Space}, {"TAB", Qt::Key_Tab}, {"ESCAPE", Qt::Key_Escape},
        {"BACKSPACE", Qt::Key_Backspace}, {"DELETE", Qt::Key_Delete},
        {"INSERT", Qt::Key_Insert}, {"HOME", Qt::Key_Home}, {"END", Qt::Key_End},
        {"PAGEUP", Qt::Key_PageUp}, {"PAGEDOWN", Qt::Key_PageDown},
        {"UP", Qt::Key_Up}, {"DOWN", Qt::Key_Down}, {"LEFT", Qt::Key_Left}, {"RIGHT", Qt::Key_Right}
    };

    if (keyMap.contains(keyStr)) {
        key = keyMap[keyStr];
        return true;
    }

    return false;
}

QString HotkeyService::formatHotkeyString(Qt::KeyboardModifiers modifiers, int key)
{
    QStringList parts;

    if (modifiers & Qt::ControlModifier) parts << "Ctrl";
    if (modifiers & Qt::ShiftModifier) parts << "Shift";
    if (modifiers & Qt::AltModifier) parts << "Alt";

    // Add key string
    if (key >= Qt::Key_F1 && key <= Qt::Key_F12) {
        parts << QString("F%1").arg(key - Qt::Key_F1 + 1);
    } else if (key >= Qt::Key_A && key <= Qt::Key_Z) {
        parts << QString(QChar(key));
    } else if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        parts << QString(QChar(key));
    } else {
        // Add other special keys as needed
        QMap<int, QString> specialKeys = {
            {Qt::Key_Space, "Space"},
            {Qt::Key_Tab, "Tab"},
            {Qt::Key_Escape, "Escape"},
            {Qt::Key_Backspace, "Backspace"},
            {Qt::Key_Delete, "Delete"},
            {Qt::Key_Insert, "Insert"},
            {Qt::Key_Home, "Home"},
            {Qt::Key_End, "End"},
            {Qt::Key_PageUp, "PageUp"},
            {Qt::Key_PageDown, "PageDown"},
            {Qt::Key_Up, "Up"},
            {Qt::Key_Down, "Down"},
            {Qt::Key_Left, "Left"},
            {Qt::Key_Right, "Right"}
        };

        if (specialKeys.contains(key)) {
            parts << specialKeys[key];
        }
    }

    return parts.join(" + ");
}

bool HotkeyService::isValidHotkeyString(const QString& hotkeyString) const
{
    if (hotkeyString.isEmpty()) return false;

    // Check for forbidden keys
    QStringList forbidden = {"LWIN", "RWIN", "FN", "ENTER"};
    QString upper = hotkeyString.toUpper();
    for (const QString& forbid : forbidden) {
        if (upper.contains(forbid)) {
            return false;
        }
    }

    return true;
}

void HotkeyService::cleanup()
{
    unregisterAllHotkeys();
}

#ifdef Q_OS_WIN
bool HotkeyService::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
{
    Q_UNUSED(result)

    if (eventType == "windows_generic_MSG") {
        MSG* msg = static_cast<MSG*>(message);
        if (msg->message == WM_HOTKEY) {
            int id = static_cast<int>(msg->wParam);
            if (m_registeredHotkeys.contains(id)) {
                emit hotkeyPressed(id);
                return true;
            }
        }
    }

    return false;
}

bool HotkeyService::registerNativeHotkey(int id, Qt::KeyboardModifiers modifiers, int key)
{
    UINT nativeModifiers = qtModifiersToNative(modifiers);
    DWORD virtualKey = qtKeyToVirtual(static_cast<Qt::Key>(key));

    if (virtualKey == 0) {
        return false;
    }

    return RegisterHotKey(nullptr, id, nativeModifiers, virtualKey);
}

bool HotkeyService::unregisterNativeHotkey(int id)
{
    return UnregisterHotKey(nullptr, id);
}

UINT HotkeyService::qtModifiersToNative(Qt::KeyboardModifiers modifiers)
{
    UINT nativeModifiers = 0;
    if (modifiers & Qt::AltModifier) nativeModifiers |= MOD_ALT;
    if (modifiers & Qt::ControlModifier) nativeModifiers |= MOD_CONTROL;
    if (modifiers & Qt::ShiftModifier) nativeModifiers |= MOD_SHIFT;
    return nativeModifiers;
}

DWORD HotkeyService::qtKeyToVirtual(Qt::Key key)
{
    switch (key) {
    case Qt::Key_A: return 'A';
    case Qt::Key_B: return 'B';
    case Qt::Key_C: return 'C';
    case Qt::Key_D: return 'D';
    case Qt::Key_E: return 'E';
    case Qt::Key_F: return 'F';
    case Qt::Key_G: return 'G';
    case Qt::Key_H: return 'H';
    case Qt::Key_I: return 'I';
    case Qt::Key_J: return 'J';
    case Qt::Key_K: return 'K';
    case Qt::Key_L: return 'L';
    case Qt::Key_M: return 'M';
    case Qt::Key_N: return 'N';
    case Qt::Key_O: return 'O';
    case Qt::Key_P: return 'P';
    case Qt::Key_Q: return 'Q';
    case Qt::Key_R: return 'R';
    case Qt::Key_S: return 'S';
    case Qt::Key_T: return 'T';
    case Qt::Key_U: return 'U';
    case Qt::Key_V: return 'V';
    case Qt::Key_W: return 'W';
    case Qt::Key_X: return 'X';
    case Qt::Key_Y: return 'Y';
    case Qt::Key_Z: return 'Z';
    case Qt::Key_0: return '0';
    case Qt::Key_1: return '1';
    case Qt::Key_2: return '2';
    case Qt::Key_3: return '3';
    case Qt::Key_4: return '4';
    case Qt::Key_5: return '5';
    case Qt::Key_6: return '6';
    case Qt::Key_7: return '7';
    case Qt::Key_8: return '8';
    case Qt::Key_9: return '9';
    case Qt::Key_F1: return VK_F1;
    case Qt::Key_F2: return VK_F2;
    case Qt::Key_F3: return VK_F3;
    case Qt::Key_F4: return VK_F4;
    case Qt::Key_F5: return VK_F5;
    case Qt::Key_F6: return VK_F6;
    case Qt::Key_F7: return VK_F7;
    case Qt::Key_F8: return VK_F8;
    case Qt::Key_F9: return VK_F9;
    case Qt::Key_F10: return VK_F10;
    case Qt::Key_F11: return VK_F11;
    case Qt::Key_F12: return VK_F12;
    case Qt::Key_Space: return VK_SPACE;
    case Qt::Key_Tab: return VK_TAB;
    case Qt::Key_Escape: return VK_ESCAPE;
    case Qt::Key_Backspace: return VK_BACK;
    case Qt::Key_Delete: return VK_DELETE;
    case Qt::Key_Insert: return VK_INSERT;
    case Qt::Key_Home: return VK_HOME;
    case Qt::Key_End: return VK_END;
    case Qt::Key_PageUp: return VK_PRIOR;
    case Qt::Key_PageDown: return VK_NEXT;
    case Qt::Key_Up: return VK_UP;
    case Qt::Key_Down: return VK_DOWN;
    case Qt::Key_Left: return VK_LEFT;
    case Qt::Key_Right: return VK_RIGHT;
    default: return 0;
    }
}

#else
// Linux/macOS implementation would go here
bool HotkeyService::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(message)
    Q_UNUSED(result)
    return false;
}

bool HotkeyService::registerNativeHotkey(int id, Qt::KeyboardModifiers modifiers, int key)
{
    Q_UNUSED(id)
    Q_UNUSED(modifiers)
    Q_UNUSED(key)
    return false;
}

bool HotkeyService::unregisterNativeHotkey(int id)
{
    Q_UNUSED(id)
    return false;
}
#endif

QStringList HotkeyService::getSupportedKeys() const
{
    return QStringList{
        // Letters
        "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M",
        "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",

        // Numbers
        "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",

        // Function keys
        "F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",

        // Special keys
        "SPACE", "TAB", "ESCAPE", "BACKSPACE", "DELETE", "INSERT",
        "HOME", "END", "PAGEUP", "PAGEDOWN", "UP", "DOWN", "LEFT", "RIGHT"
    };
}

QStringList HotkeyService::getSupportedModifiers() const
{
    return QStringList{"Ctrl", "Shift", "Alt"};
}
