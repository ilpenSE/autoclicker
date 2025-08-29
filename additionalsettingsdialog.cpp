#include "additionalsettingsdialog.h"

#include <QCursor>
#include <QRegularExpression>

#include "languagemanager.h"
#include "ui_additionalsettingsdialog.h"

AdditionalSettingsDialog::AdditionalSettingsDialog(const MacroAction& action,
                                                   QWidget* parent)
    : QDialog(parent), ui(new Ui::AdditionalSettingsDialog), m_action(action) {
  ui->setupUi(this);
  setupUI();
  loadActionData();
  loadLanguage();

  // Language manager bağlantısı
  connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this,
          &AdditionalSettingsDialog::loadLanguage);
}

AdditionalSettingsDialog::~AdditionalSettingsDialog() { delete ui; }

void AdditionalSettingsDialog::setupUI() {
  // Current position checkbox bağlantısı
  connect(ui->checkBoxCurrentPos, &QCheckBox::toggled, this,
          &AdditionalSettingsDialog::onCurrentPosToggled);

  // Dialog başlığı
  setWindowTitle(trans("additionals"));
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

  // Position spinbox'larını current pos durumuna göre ayarla
  ui->spinBoxX->setEnabled(!useCurrentPos);
  ui->spinBoxY->setEnabled(!useCurrentPos);

  // Timing ayarları (optional fields)
  ui->spinBoxHoldDuration->setValue(
      m_action.hold_duration.has_value() ? m_action.hold_duration.value() : 0);
  ui->spinBoxHoverDuration->setValue(m_action.hover_duration.has_value()
                                         ? m_action.hover_duration.value()
                                         : 0);

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
  if (ui->checkBoxCurrentPos->isChecked()) {
    // Mevcut mouse pozisyonunu al
    QPoint currentPos = QCursor::pos();
    updatedAction.position =
        std::make_optional(formatPosition(currentPos.x(), currentPos.y()));
    updatedAction.current_pos = true;
  } else {
    // Manuel girilen pozisyonu kullan
    updatedAction.position = std::make_optional(
        formatPosition(ui->spinBoxX->value(), ui->spinBoxY->value()));
    updatedAction.current_pos = false;
  }

  // Timing ayarlarını güncelle (optional fields)
  int holdDur = ui->spinBoxHoldDuration->value();
  updatedAction.hold_duration =
      holdDur > 0 ? std::make_optional(holdDur) : std::nullopt;

  int hoverDur = ui->spinBoxHoverDuration->value();
  updatedAction.hover_duration =
      hoverDur > 0 ? std::make_optional(hoverDur) : std::nullopt;

  // Action ayarlarını güncelle
  updatedAction.click_count = ui->spinBoxClickCount->value();  // default 1

  // Key name (optional field)
  QString keyName = ui->lineEditKeyName->text().trimmed();
  updatedAction.key_name =
      keyName.isEmpty() ? std::nullopt : std::make_optional(keyName);

  return updatedAction;
}

void AdditionalSettingsDialog::onCurrentPosToggled(bool enabled) {
  // Current position işaretliyse X,Y spinbox'larını devre dışı bırak
  ui->spinBoxX->setEnabled(!enabled);
  ui->spinBoxY->setEnabled(!enabled);

  if (enabled) {
    // Mevcut mouse pozisyonunu göster
    QPoint currentPos = QCursor::pos();
    ui->spinBoxX->setValue(currentPos.x());
    ui->spinBoxY->setValue(currentPos.y());
  }
}

void AdditionalSettingsDialog::parsePosition(const QString& positionStr, int& x,
                                             int& y) {
  // "x, y" formatındaki string'i parse et
  QRegularExpression regex(R"(\s*(-?\d+)\s*,\s*(-?\d+)\s*)");
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
  setWindowTitle(tr("Additional Settings"));

  ui->groupBoxPosition->setTitle(tr("Position Settings"));
  ui->labelPosition->setText(tr("Position (X, Y):"));
  ui->labelCurrentPos->setText(tr("Use Current Position:"));
  ui->checkBoxCurrentPos->setText(tr("Current mouse position"));

  ui->groupBoxTiming->setTitle(tr("Timing Settings"));
  ui->labelHoldDuration->setText(tr("Hold Duration (ms):"));
  ui->labelHoverDuration->setText(tr("Hover Duration (ms):"));

  ui->groupBoxAction->setTitle(tr("Action Settings"));
  ui->labelClickCount->setText(tr("Click Count:"));
  ui->labelKeyName->setText(tr("Key Name:"));
  ui->lineEditKeyName->setPlaceholderText(
      tr("Enter key name (e.g., A, Space, Enter)"));

  // Tooltip'ler
  ui->spinBoxX->setToolTip(tr("X coordinate"));
  ui->spinBoxY->setToolTip(tr("Y coordinate"));
  ui->checkBoxCurrentPos->setToolTip(tr("Use current mouse cursor position"));
  ui->spinBoxHoldDuration->setToolTip(
      tr("How long to hold the mouse button (0 = no hold)"));
  ui->spinBoxHoverDuration->setToolTip(
      tr("How long to hover before action (0 = no hover)"));
  ui->spinBoxClickCount->setToolTip(tr("Number of clicks to perform"));
  ui->lineEditKeyName->setToolTip(tr("Key to press (for KEY action type)"));
}
