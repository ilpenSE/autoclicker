#include "additionalsettingsdialog.h"

#include <QCursor>
#include <QRegularExpression>
#include <QKeyEvent>
#include <QTimer>

#include "LoggerStream.h"
#include "languagemanager.h"
#include "ui_additionalsettingsdialog.h"
#include "thememanager.h"

AdditionalSettingsDialog::AdditionalSettingsDialog(const MacroAction& action,
                                                   QWidget* parent)
    : QDialog(parent), ui(new Ui::AdditionalSettingsDialog), m_action(action) {
  ui->setupUi(this);
  ui->labelPosInfo->setVisible(false);

  setupUI();
  loadActionData();
  loadLanguage();

  // Language manager bağlantısı
  connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this,
          &AdditionalSettingsDialog::loadLanguage);
  // Connect to theme changes
  connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this,
          &AdditionalSettingsDialog::onThemeChanged);
}

AdditionalSettingsDialog::~AdditionalSettingsDialog() { delete ui; }

void AdditionalSettingsDialog::setupUI() {
  connect(ui->btnRecordPos, &QPushButton::clicked, this, [this]() {
    m_recordingPos = true;
    ui->labelPosInfo->setText(trans("Cursor tracking started, click Ctrl + S to stop."));
    ui->labelPosInfo->setVisible(true);
  });

  // Dialog başlığı
  setWindowTitle(trans("Additional Settings"));

  setupDynamicIcons();
}

void AdditionalSettingsDialog::keyPressEvent(QKeyEvent* event) {
  if (m_recordingPos &&
      (event->modifiers() & Qt::ControlModifier) &&
      event->key() == Qt::Key_S) {
    QPoint pos = QCursor::pos();
    ui->spinBoxX->setValue(pos.x());
    ui->spinBoxY->setValue(pos.y());
    m_recordingPos = false;
    ui->labelPosInfo->setText(trans("Cursor Position set!"));
    QTimer::singleShot(1000, this, [this](){
      ui->labelPosInfo->setVisible(false);
    });
    return;
  }
  QDialog::keyPressEvent(event);
}

QString AdditionalSettingsDialog::trans(QString str) {
  return QApplication::translate("MainWindow", qPrintable(str));
}

void AdditionalSettingsDialog::loadActionData() {
  // Position verilerini yükle
  if (m_action.position.has_value() && !m_action.position.value().isEmpty()) {
    int x = 0, y = 0;
    parsePosition(m_action.position.value(), x, y);
    ui->spinBoxX->setValue(x);
    ui->spinBoxY->setValue(y);
  }

  // Current position ayarı (bool field, default true)
  bool useCurrentPos =
      m_action.current_pos.has_value() ? m_action.current_pos.value() : false;
  ui->checkBoxCurrentPos->setChecked(useCurrentPos);

  // Timing ayarları (optional fields)
  ui->spinBoxHoldDuration->setValue(
      m_action.hold_duration.has_value() ? m_action.hold_duration.value() : 0);
  // Action ayarları (default 1)
  ui->spinBoxClickCount->setValue(m_action.click_count);

  // Key name (optional field)
  if (m_action.key_name.has_value()) {
    ui->lineEditKeyName->setText(m_action.key_name.value());
  }
}

MacroAction AdditionalSettingsDialog::getUpdatedAction() {
  MacroAction updatedAction = m_action;

  // Position bilgilerini güncelle
  updatedAction.position = std::make_optional(
      formatPosition(ui->spinBoxX->value(), ui->spinBoxY->value()));
  updatedAction.current_pos = ui->checkBoxCurrentPos->checkState();

  // Timing ayarlarını güncelle (optional fields)
  int holdDur = ui->spinBoxHoldDuration->value();
  updatedAction.hold_duration =
      holdDur > 0 ? std::make_optional(holdDur) : std::nullopt;

  // Action ayarlarını güncelle
  updatedAction.click_count = ui->spinBoxClickCount->value();  // default 1

  // Key name (optional field)
  QString keyName = ui->lineEditKeyName->text().trimmed();
  updatedAction.key_name =
      keyName.isEmpty() ? std::nullopt : std::make_optional(keyName);

  return updatedAction;
}

void AdditionalSettingsDialog::parsePosition(const QString& positionStr, int& x,
                                             int& y) {
  // "x, y" formatındaki string'i parse et
  static QRegularExpression regex(R"(\s*(-?\d+)\s*,\s*(-?\d+)\s*)");
  QRegularExpressionMatch match = regex.match(positionStr);

  if (match.hasMatch()) {
    x = match.captured(1).toInt();
    y = match.captured(2).toInt();
  } else {
    // Parse edilemezse varsayılan değerler
    x = 0;
    y = 0;
  }
}

QString AdditionalSettingsDialog::formatPosition(int x, int y) {
  return QString("%1, %2").arg(x).arg(y);
}

void AdditionalSettingsDialog::loadLanguage() {
  // UI çevirileri
  setWindowTitle(trans("Additional Settings"));

  ui->groupBoxPosition->setTitle(trans("Position Settings"));
  ui->labelPosition->setText(trans("Position (X, Y):"));
  ui->labelCurrentPos->setText(trans("Use Current Position:"));
  ui->checkBoxCurrentPos->setText(trans("Current mouse position"));

  ui->groupBoxTiming->setTitle(trans("Timing Settings"));
  ui->labelHoldDuration->setText(trans("Hold Duration (ms):"));

  ui->groupBoxAction->setTitle(trans("Action Settings"));
  ui->labelClickCount->setText(trans("Click Count:"));
  ui->labelKeyName->setText(trans("Key Name:"));
  ui->lineEditKeyName->setPlaceholderText(
      trans("Enter key name (e.g., A, Space, Enter)"));

  // Tooltip'ler
  ui->spinBoxX->setToolTip(trans("X coordinate"));
  ui->spinBoxY->setToolTip(trans("Y coordinate"));
  ui->checkBoxCurrentPos->setToolTip(trans("Use current mouse cursor position"));
  ui->spinBoxHoldDuration->setToolTip(
      trans("How long to hold the mouse button (0 = no hold)"));
  ui->spinBoxClickCount->setToolTip(trans("Number of clicks to perform"));
  ui->lineEditKeyName->setToolTip(trans("Key to press (for KEY action type)"));
  ui->btnRecordPos->setToolTip(trans("Starts recording cursor position"));
  ui->btnSelectKey->setToolTip(trans("Stops the key capture"));
}

// DYNAMIC ICONS
void AdditionalSettingsDialog::onThemeChanged() {
  // This slot is called when theme changes
  // Icons are automatically updated by ThemeManager
  thinfo() << "Theme changed, icons updated automatically";

  refreshIcons();
}

void AdditionalSettingsDialog::refreshIcons() {
  // Force refresh all icons if needed
  ThemeManager::instance().refreshAllIcons();
}
void AdditionalSettingsDialog::setupDynamicIcons() {
  // Use assets/icons from the project directory
  QString iconsPath = ":/assets/icons";  // Resource path

  // record cursor pos
  if (ui->btnRecordPos) {
    ThemeManager::instance().setupDynamicButton(
        ui->btnRecordPos, iconsPath + "/cursor.svg", QSize(16, 16));
  }
  // select key
  if (ui->btnSelectKey) {
    ThemeManager::instance().setupDynamicButton(
        ui->btnSelectKey, iconsPath + "/select.svg", QSize(16,16));
  }

  thinfo() << "Dynamic icons setup completed";
}

void AdditionalSettingsDialog::on_btnSelectKey_clicked()
{
  if (ui->lineEditKeyName->isCapturing()) {
    // Capture modundaysa, hotkey'i tamamla
    ui->lineEditKeyName->stopCapture();
  }
  ui->lineEditKeyName->clearFocus();  // fokus kalksın
}

