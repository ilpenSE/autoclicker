#include "macroselectionwin.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>

#include "LoggerStream.h"
#include "hotkeylineedit.h"
#include "languagemanager.h"
#include "logger.h"
#include "macromanager.h"
#include "thememanager.h"
#include "ui_macroselectionwin.h"

MacroSelectionWin::MacroSelectionWin(QVector<Macro>& macros, int activeMacroId,
                                     QWidget* parent)
    : QDialog(parent),
      m_macros(macros),
      activeMacroId(activeMacroId),
      ui(new Ui::MacroSelectionWin) {
  ui->setupUi(this);

  connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this,
          &MacroSelectionWin::retranslateUi);

  connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this,
          &MacroSelectionWin::onThemeChanged);

  QTimer::singleShot(0, this, &MacroSelectionWin::setupDynamicIcons);

  // tablo ayarlamaları
  QTimer::singleShot(0, this, &MacroSelectionWin::adjustTableColumns);
  connect(ui->macrosTable, &QTableWidget::itemChanged, this,
          &MacroSelectionWin::onItemChanged);
  ui->macrosTable->verticalHeader()->setVisible(false);

  loadLang();

  for (const Macro& m : macros) {
    addMacro(m, activeMacroId == m.id);
  }
}

void MacroSelectionWin::adjustTableColumns() {
  int tableWidth = ui->macrosTable->viewport()->width();

  ui->macrosTable->setColumnWidth(0, tableWidth * 0.08);  // ID
  ui->macrosTable->setColumnWidth(1, tableWidth * 0.30);  // NAME
  ui->macrosTable->setColumnWidth(2, tableWidth * 0.40);  // DESCRIPTION
  ui->macrosTable->setColumnWidth(3, tableWidth * 0.22);  // HOTKEY
}

void MacroSelectionWin::resizeEvent(QResizeEvent* event) {
  QDialog::resizeEvent(event);
  adjustTableColumns();
}

void MacroSelectionWin::setupDynamicIcons() {
  // Use assets/icons from the project directory
  QString iconsPath = ":/assets/icons";  // Resource path

  // Setup QActions with dynamic icons
  // create
  if (ui->btnCreate) {
    ThemeManager::instance().setupDynamicButton(
        ui->btnCreate, iconsPath + "/add.svg", QSize(24, 24));
  }
  // delete
  if (ui->btnDelete) {
    ThemeManager::instance().setupDynamicButton(
        ui->btnDelete, iconsPath + "/delete.svg", QSize(24, 24));
  }
  // cancel
  if (ui->btnCancel) {
    ThemeManager::instance().setupDynamicButton(
        ui->btnCancel, iconsPath + "/cancel.svg", QSize(24, 24));
  }
  // select
  if (ui->btnSelect) {
    ThemeManager::instance().setupDynamicButton(
        ui->btnSelect, iconsPath + "/select.svg", QSize(24, 24));
  }

  sinfo() << "Dynamic icons setup completed";
}

void MacroSelectionWin::onThemeChanged() { refreshIcons(); }

void MacroSelectionWin::refreshIcons() {
  ThemeManager::instance().refreshAllIcons();
}

void MacroSelectionWin::addMacro(Macro macro, bool isSelected) {
  int row = ui->macrosTable->rowCount();
  ui->macrosTable->insertRow(row);

  // --- ID sütunu (salt okunur, ortalanmış) ---
  QTableWidgetItem* idItem = new QTableWidgetItem(QString::number(macro.id));
  idItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
  idItem->setTextAlignment(Qt::AlignCenter);
  ui->macrosTable->setItem(row, 0, idItem);

  // --- Name sütunu (normal text) ---
  QTableWidgetItem* nameItem = new QTableWidgetItem(macro.name);
  if (macro.name == "DEFAULT") {
    nameItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
  }
  ui->macrosTable->setItem(row, 1, nameItem);

  // --- Description sütunu (normal text) ---
  QTableWidgetItem* descItem = new QTableWidgetItem(macro.description);
  if (macro.name == "DEFAULT") {
    descItem->setFlags(idItem->flags() & ~Qt::ItemIsEditable);
  }
  ui->macrosTable->setItem(row, 2, descItem);

  // --- Hotkey sütunu (HotkeyLineEdit widget) ---
  HotkeyLineEdit* hotkeyWidget = new HotkeyLineEdit();
  hotkeyWidget->setHotkey(macro.hotkey);
  if (macro.name == "DEFAULT") {
    hotkeyWidget->setReadOnlyMode(true);
  }
  hotkeyWidget->setTableMode(true);
  ui->macrosTable->setCellWidget(row, 3, hotkeyWidget);
  // Hotkey widget bağlantıları
  connect(hotkeyWidget, &HotkeyLineEdit::hotkeyReady, this,
          &MacroSelectionWin::onHotkeyReady);
  connect(hotkeyWidget, &HotkeyLineEdit::hotkeyChanged, this,
          &MacroSelectionWin::onHotkeyChanged);

  if (isSelected) {
    ui->macrosTable->setCurrentCell(row, 0);
    ui->macrosTable->selectRow(row);
  }
}
void MacroSelectionWin::onItemChanged(QTableWidgetItem* item) {
  if (!item) return;

  int row = item->row();
  int column = item->column();

  if (row < 0 || row >= m_macros.size()) return;

  // ID sütunu değiştirilemez
  if (column == 0) return;

  Macro& macro = m_macros[row];
  QString newValue = item->text().trimmed();

  if (column == 1) {                     // Name sütunu
    if (macro.name == newValue) return;  // Değişiklik yoksa çık

    QString oldName = macro.name;
    macro.name = newValue;

    // SQL güncelleme
    QString error;
    bool success =
        MacroManager::instance().updateMacroName(macro.id, newValue, &error);

    if (!success) {
      // Hata varsa eski değere geri dön
      macro.name = oldName;
      item->setText(oldName);
      QMessageBox::critical(this, trans("error"), trans(error));
      merr() << "Macro name update failed: " << error;
    } else {
      minfo() <<
          QString("Macro name updated: ID %1, new name: %2")
              .arg(macro.id)
              .arg(newValue);
    }

  } else if (column == 2) {  // Description sütunu
    if (macro.description == newValue) return;

    QString oldDesc = macro.description;
    macro.description = newValue;

    // SQL güncelleme
    QString error;
    bool success = MacroManager::instance().updateMacroDescription(
        macro.id, newValue, &error);

    if (!success) {
      // Hata varsa eski değere geri dön
      macro.description = oldDesc;
      item->setText(oldDesc);
      QMessageBox::critical(this, trans("error"), trans(error));
      merr() << "Macro description update failed: " << error;
    } else {
      minfo() <<
          QString("Macro description updated: ID %1, new desc: %2")
              .arg(macro.id)
              .arg(newValue);
    }
  }
}

void MacroSelectionWin::onHotkeyReady(const QString& hotkey) {
  // Hangi row'daki widget'tan geldiğini bul
  HotkeyLineEdit* senderWidget = qobject_cast<HotkeyLineEdit*>(sender());
  if (!senderWidget) return;

  int row = -1;
  for (int i = 0; i < ui->macrosTable->rowCount(); ++i) {
    if (ui->macrosTable->cellWidget(i, 3) == senderWidget) {
      row = i;
      break;
    }
  }

  if (row < 0 || row >= m_macros.size()) return;

  Macro& macro = m_macros[row];
  QString oldHotkey = macro.hotkey;

  if (oldHotkey == hotkey) return;  // Değişiklik yoksa çık

  macro.hotkey = hotkey;

  // SQL güncelleme
  QString error;
  bool success =
      MacroManager::instance().updateMacroHotkey(macro.id, hotkey, &error);

  if (!success) {
    // Hata varsa eski değere geri dön
    macro.hotkey = oldHotkey;
    senderWidget->setHotkey(oldHotkey);
    QMessageBox::critical(this, trans("error"), trans(error));
    merr() << "Macro hotkey update failed: " << error;
  } else {
    minfo() <<
        QString("Macro hotkey updated: ID %1, new hotkey: %2")
            .arg(macro.id)
            .arg(hotkey);
  }
}

void MacroSelectionWin::onHotkeyChanged() { info() << "Hotkey Changed!"; }

QString MacroSelectionWin::trans(const QString& key) {
  return QApplication::translate("MainWindow", qPrintable(key));
}

void MacroSelectionWin::loadLang() {
  this->setWindowTitle(trans("macro selection title"));
  ui->btnCancel->setText(trans("cancel"));
  ui->btnCreate->setText(trans("create"));
  ui->btnSelect->setText(trans("select"));
  ui->btnDelete->setText(trans("delete"));

  ui->macrosTable->setHorizontalHeaderLabels(
      QStringList() << "ID" << trans("name") << trans("description")
                    << trans("hotkey"));
}

void MacroSelectionWin::retranslateUi() {
  ui->retranslateUi(this);
  loadLang();
}

MacroSelectionWin::~MacroSelectionWin() { delete ui; }

void MacroSelectionWin::on_btnSelect_clicked() {
  if (ui->macrosTable->selectedItems().count() <= 0) {
    QMessageBox::critical(this, trans("error"),
                          trans("Please select a macro."));
    return;
  }

  int selectedRow = ui->macrosTable->currentRow();

  if (selectedRow >= 0 && selectedRow < m_macros.size()) {
    activeMacroId = m_macros[selectedRow].id;
    minfo() <<
        "(From Macro Selection Window) New active macro ID: " << activeMacroId;
    accept();
  } else {
    QMessageBox::critical(this, trans("error"), trans("Invalid selection."));
  }

  accept();
}

void MacroSelectionWin::on_btnCancel_clicked() { reject(); }

void MacroSelectionWin::on_btnDelete_clicked() {
  if (ui->macrosTable->selectedItems().count() <= 0) {
    QMessageBox::critical(this, trans("error"),
                          trans("Please select a macro."));
    return;
  }

  int selectedRow = ui->macrosTable->currentRow();

  if (selectedRow < 0 || selectedRow >= m_macros.size()) {
    QMessageBox::critical(this, trans("error"), trans("Invalid selection."));
    return;
  }

  // QVector'den gerçek makroyu al
  Macro macroToDelete = m_macros[selectedRow];

  QString err;
  // SQL'dan sil
  if (!MacroManager::instance().deleteMacro(macroToDelete.id, &err)) {
    Logger::instance().mError(err);
    QMessageBox::critical(this, trans("error"), trans(err));
    return;
  }

  // QVector'den sil (index kullanarak)
  m_macros.removeAt(selectedRow);

  // UI'dan sil
  ui->macrosTable->removeRow(selectedRow);

  minfo() << "Macro deleted successfully. ID: " << macroToDelete.id;
}

void MacroSelectionWin::on_btnCreate_clicked() {
  QString err;

  bool ok;
  QString name =
      QInputDialog::getText(this,                           // parent
                            trans("new macro name"),        // dialog başlığı
                            trans("enter new macro name"),  // label
                            QLineEdit::Normal,              // input type
                            "",                             // default text
                            &ok                             // okPressed flag
      );

  if (!ok) return;

  int id = MacroManager::instance().createMacro(name, trans("No Description"),
                                                "DEF", &err);

  if (!err.isEmpty()) {
    QMessageBox::critical(this, trans("error"), trans(err));
  }

  if (id > 0) {
    // oluşturulan makroyu al
    std::optional<Macro> qmacro = MacroManager::instance().getMacroById(id);
    Macro macro;
    if (qmacro.has_value()) {
      macro = qmacro.value();
    }

    // makroyu m_macros'a kaydet
    m_macros.append(macro);

    // UI güncelle
    addMacro(macro);

    minfo() << "Macro created successfully: " << name;
  } else {
    merr() << err;
  }
}
