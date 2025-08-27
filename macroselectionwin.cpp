#include "macroselectionwin.h"

#include <QInputDialog>
#include <QLineEdit>
#include <QMessageBox>
#include <QTimer>

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

  Logger::instance().sInfo("Dynamic icons setup completed");
}

void MacroSelectionWin::onThemeChanged() {
  // This slot is called when theme changes
  // Icons are automatically updated by ThemeManager
  Logger::instance().sInfo("Theme changed, icons updated automatically");

  // You can add additional theme-related updates here if needed
  refreshIcons();
}

void MacroSelectionWin::refreshIcons() {
  // Force refresh all icons if needed
  ThemeManager::instance().refreshAllIcons();
}

void MacroSelectionWin::addMacro(Macro macro, bool isSelected) {
  int row = ui->macrosTable->rowCount();
  ui->macrosTable->insertRow(row);

  ui->macrosTable->setItem(row, 0,
                           new QTableWidgetItem(QString::number(macro.id)));
  ui->macrosTable->setItem(row, 1, new QTableWidgetItem(macro.name));
  ui->macrosTable->setItem(row, 2, new QTableWidgetItem(macro.description));
  ui->macrosTable->setItem(row, 3, new QTableWidgetItem(macro.hotkey));

  if (isSelected) {
    ui->macrosTable->setCurrentCell(row, 0);
    ui->macrosTable->selectRow(row);
  }
}

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
    Logger::instance().mInfo(
        "(From Macro Selection Window) New active macro ID: " +
        QString::number(activeMacroId));
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

  Logger::instance().mInfo("Macro deleted successfully. ID: " +
                           QString::number(macroToDelete.id));
}

void MacroSelectionWin::on_btnCreate_clicked() {
  QString err;

  bool ok;
  QString name = QInputDialog::getText(
      this,                           // parent
      trans("new macro name"),        // dialog başlığı
      trans("enter new macro name"),  // label
      QLineEdit::Normal,              // input type (Normal / Password / NoEcho)
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

    Logger::instance().mInfo("Macro created successfully: " + name);
  } else {
    Logger::instance().mError(err);
  }
}
