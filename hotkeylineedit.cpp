#include "hotkeylineedit.h"

#include <QApplication>
#include <QDebug>
#include <QVBoxLayout>

HotkeyLineEdit::HotkeyLineEdit(QWidget* parent)
    : QLineEdit(parent),
      m_mainKey(-1),
      m_capturing(false),
      m_finalHotkey(""),
      m_updateTimer(nullptr),
      m_dummyFocusWidget(nullptr),
      m_inUpdateDisplay(false),
      m_inFocusEvent(false) {
  // Timer
  m_updateTimer = new QTimer(this);
  m_updateTimer->setSingleShot(true);
  m_updateTimer->setInterval(50);
  connect(m_updateTimer, &QTimer::timeout, this,
          &HotkeyLineEdit::updateDisplay);

  // Dummy focus widget
  m_dummyFocusWidget = new QLabel(this);
  m_dummyFocusWidget->setFocusPolicy(Qt::StrongFocus);
  m_dummyFocusWidget->setVisible(false);
  m_dummyFocusWidget->setFixedSize(1, 1);
  m_dummyFocusWidget->move(-10, -10);

  // Widget settings
  setReadOnly(true);
  setPlaceholderText("Click to set hotkey...");
  setStyleSheet("QLineEdit { padding: 6px 10px; }");
}

HotkeyLineEdit::~HotkeyLineEdit() {
  if (m_updateTimer) {
    m_updateTimer->stop();
    m_updateTimer->deleteLater();
    m_updateTimer = nullptr;
  }
}

void HotkeyLineEdit::setHotkey(const QString& hotkey) {
  m_finalHotkey = hotkey;
  setText(hotkey.isEmpty() ? "" : hotkey);
  if (!hotkey.isEmpty()) {
    setPlaceholderText("Click to change hotkey...");
  }
}

QString HotkeyLineEdit::getHotkey() const { return m_finalHotkey; }

void HotkeyLineEdit::startCapture() {
  if (m_capturing) return;

  m_capturing = true;
  clearCapture();
  setText("+ ...");
  setPlaceholderText("Press keys and click Select button");
  // setFocus(); // kaldırıldı
}

void HotkeyLineEdit::stopCapture() {
  if (!m_capturing) return;
  finalizeCapturedHotkey();
}

void HotkeyLineEdit::acceptHotkey() {
  stopCapture();  // finalize
  clearFocus();   // focus lineedit’ten kalksın
}

void HotkeyLineEdit::cancelCapture() {
  m_capturing = false;
  setText(m_finalHotkey);
  clearCapture();
  clearFocus();
}

void HotkeyLineEdit::finalizeCapturedHotkey() {
  if (!m_capturing) return;

  QString newHotkey = formatCurrentHotkey();

  if (!newHotkey.isEmpty()) {
    m_finalHotkey = newHotkey;
    setText(m_finalHotkey);
    setPlaceholderText("Click to change hotkey...");
    emit hotkeyReady(m_finalHotkey);
    emit hotkeyChanged();
  } else {
    setText(m_finalHotkey);
  }

  m_capturing = false;
  clearCapture();

  if (m_dummyFocusWidget) {
    m_dummyFocusWidget->setFocus();
  }
}

void HotkeyLineEdit::keyPressEvent(QKeyEvent* event) {
  if (!m_capturing) {
    startCapture();
    return;
  }
  if (event->isAutoRepeat()) return;

  int key = event->key();

  // ESC cancel
  if (key == Qt::Key_Escape) {
    cancelCapture();
    return;
  }

  if (!isValidKey(key)) return;

  if (isModifierKey(key)) {
    m_modifiers.insert(key);  // release’te silmiyoruz
  } else {
    if (m_mainKey == -1) {
      m_mainKey = key;  // ilk non-modifier key’i al
    }
  }

  updateDisplay();
}

void HotkeyLineEdit::keyReleaseEvent(QKeyEvent* event) {
  if (!m_capturing) return;
  if (event->isAutoRepeat()) return;

  // Artık release’te modifiyer/ana tuşu silmiyoruz
}

void HotkeyLineEdit::focusInEvent(QFocusEvent* event) {
  QLineEdit::focusInEvent(event);
  if (!m_capturing) {
    startCapture();
  }
}

void HotkeyLineEdit::focusOutEvent(QFocusEvent* event) {
  QLineEdit::focusOutEvent(event);
  if (m_capturing) {
    acceptHotkey();
  }
}

void HotkeyLineEdit::mousePressEvent(QMouseEvent* e) {
  QLineEdit::mousePressEvent(e);
  startCapture();  // sadece kullanıcı tıkladığında başlat
}

void HotkeyLineEdit::updateDisplay() {
  if (!m_capturing || m_inUpdateDisplay) return;
  m_inUpdateDisplay = true;

  const QString current = formatCurrentHotkey();
  setText(current.isEmpty() ? "+ ..." : current);

  m_inUpdateDisplay = false;
}

QString HotkeyLineEdit::keyToString(int key) const {
  switch (key) {
    case Qt::Key_Control:
      return "Ctrl";
    case Qt::Key_Shift:
      return "Shift";
    case Qt::Key_Alt:
      return "Alt";
    case Qt::Key_Meta:
      return "Meta";
  }

  if (key >= Qt::Key_F1 && key <= Qt::Key_F24)
    return QString("F%1").arg(key - Qt::Key_F1 + 1);

  if (key >= Qt::Key_A && key <= Qt::Key_Z) return QString(QChar(key));

  if (key >= Qt::Key_0 && key <= Qt::Key_9) return QString(QChar(key));

  switch (key) {
    case Qt::Key_Space:
      return "Space";
    case Qt::Key_Tab:
      return "Tab";
    case Qt::Key_Backspace:
      return "Backspace";
    case Qt::Key_Delete:
      return "Delete";
    case Qt::Key_Insert:
      return "Insert";
    case Qt::Key_Home:
      return "Home";
    case Qt::Key_End:
      return "End";
    case Qt::Key_PageUp:
      return "PageUp";
    case Qt::Key_PageDown:
      return "PageDown";
    case Qt::Key_Up:
      return "Up";
    case Qt::Key_Down:
      return "Down";
    case Qt::Key_Left:
      return "Left";
    case Qt::Key_Right:
      return "Right";
    default:
      return QString();
  }
}

bool HotkeyLineEdit::isModifierKey(int key) const {
  return key == Qt::Key_Control || key == Qt::Key_Shift || key == Qt::Key_Alt ||
         key == Qt::Key_Meta;
}

bool HotkeyLineEdit::isValidKey(int key) const {
  if (key == Qt::Key_Enter || key == Qt::Key_Return || key == Qt::Key_Meta ||
      key == Qt::Key_unknown)
    return false;

  if (isModifierKey(key)) return true;

  if (key >= Qt::Key_F1 && key <= Qt::Key_F24) return true;
  if ((key >= Qt::Key_A && key <= Qt::Key_Z) ||
      (key >= Qt::Key_0 && key <= Qt::Key_9))
    return true;

  switch (key) {
    case Qt::Key_Space:
    case Qt::Key_Tab:
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
    case Qt::Key_Insert:
    case Qt::Key_Home:
    case Qt::Key_End:
    case Qt::Key_PageUp:
    case Qt::Key_PageDown:
    case Qt::Key_Up:
    case Qt::Key_Down:
    case Qt::Key_Left:
    case Qt::Key_Right:
      return true;
    default:
      return false;
  }
}

QString HotkeyLineEdit::formatCurrentHotkey() const {
  QStringList parts;
  if (m_modifiers.contains(Qt::Key_Control)) parts << "Ctrl";
  if (m_modifiers.contains(Qt::Key_Shift)) parts << "Shift";
  if (m_modifiers.contains(Qt::Key_Alt)) parts << "Alt";

  if (m_mainKey != -1) {
    QString mainKeyStr = keyToString(m_mainKey);
    if (!mainKeyStr.isEmpty()) parts << mainKeyStr;
  }
  return parts.join(" + ");
}

void HotkeyLineEdit::clearCapture() {
  m_pressedKeys.clear();
  m_modifiers.clear();
  m_mainKey = -1;
}
