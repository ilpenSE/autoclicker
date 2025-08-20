#include "macroselectionwin.h"
#include "ui_macroselectionwin.h"

#include <QMessageBox>
#include <QTimer>

#include "logger.h"
#include "macromanager.h"

MacroSelectionWin::MacroSelectionWin(QVector<Macro>& macros, QWidget *parent)
    : QDialog(parent)
    , m_macros(macros)
    , ui(new Ui::MacroSelectionWin)
{
    ui->setupUi(this);

    // tablo ayarlamaları
    QTimer::singleShot(0, this, &MacroSelectionWin::adjustTableColumns);
    ui->macrosTable->verticalHeader()->setVisible(false);

    for (const Macro& m : macros) {
        addMacro(m.name, m.description, m.hotkey);
    }
}

void MacroSelectionWin::adjustTableColumns()
{
    int tableWidth = ui->macrosTable->viewport()->width();

    ui->macrosTable->setColumnWidth(0, tableWidth * 0.10); // ID
    ui->macrosTable->setColumnWidth(1, tableWidth * 0.30); // NAME
    ui->macrosTable->setColumnWidth(2, tableWidth * 0.40); // DESCRIPTION
    ui->macrosTable->setColumnWidth(3, tableWidth * 0.20); // HOTKEY
}

void MacroSelectionWin::resizeEvent(QResizeEvent* event)
{
    QDialog::resizeEvent(event);
    adjustTableColumns();
}

void MacroSelectionWin::addMacro(const QString& name, const QString& desc, const QString& htk) {
    int row = ui->macrosTable->rowCount();
    ui->macrosTable->insertRow(row);

    ui->macrosTable->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
    ui->macrosTable->setItem(row, 1, new QTableWidgetItem(name));
    ui->macrosTable->setItem(row, 2, new QTableWidgetItem(desc));
    ui->macrosTable->setItem(row, 3, new QTableWidgetItem(htk));
}


void MacroSelectionWin::retranslateUi() {
    ui->retranslateUi(this);
}

MacroSelectionWin::~MacroSelectionWin()
{
    delete ui;
}

void MacroSelectionWin::on_btnSelect_clicked()
{
    if (ui->macrosTable->selectedItems().count() <= 0) {
        QMessageBox::critical(this, "Error", "Please select a macro.");
        return;
    }

    int selectedRow = ui->macrosTable->currentRow();

    if (selectedRow >= 0 && selectedRow < m_macros.size()) {
        activeMacroId = m_macros[selectedRow].id;
        Logger::instance().mInfo("(From Macro Selection Window) New active macro ID: " + QString::number(activeMacroId));
        accept();
    } else {
        QMessageBox::critical(this, "Error", "Invalid selection.");
    }

    accept();
}


void MacroSelectionWin::on_btnCancel_clicked()
{
    reject();
}


void MacroSelectionWin::on_btnDelete_clicked()
{
    if (ui->macrosTable->selectedItems().count() <= 0) {
        QMessageBox::critical(this, "Error", "Please select a macro.");
        return;
    }

    int selectedRow = ui->macrosTable->currentRow();

    if (selectedRow < 0 || selectedRow >= m_macros.size()) {
        QMessageBox::critical(this, "Error", "Invalid selection.");
        return;
    }

    // QVector'den gerçek makroyu al
    Macro macroToDelete = m_macros[selectedRow];

    QString err;
    // SQL'dan sil
    if (!MacroManager::instance().deleteMacro(macroToDelete.id, &err)) {
        Logger::instance().mError(err);
        QMessageBox::critical(this, "Error", "Failed to delete macro: " + err);
        return;
    }

    // QVector'den sil (index kullanarak)
    m_macros.removeAt(selectedRow);

    // UI'dan sil
    ui->macrosTable->removeRow(selectedRow);

    // ID sütunlarını yeniden numaralandır (görsel için)
    for (int i = 0; i < ui->macrosTable->rowCount(); ++i) {
        ui->macrosTable->item(i, 0)->setText(QString::number(i + 1));
    }

    Logger::instance().mInfo("Macro deleted successfully. ID: " + QString::number(macroToDelete.id));
}

void MacroSelectionWin::on_btnCreate_clicked()
{
    QString err;

    int no = m_macros.count();
    QString name = "New Macro " + QString::number(no);
    int id = MacroManager::instance().createMacro(name, "No Description", "DEF", &err);

    if (err == "A macro with this name exists!") {
        QMessageBox::critical(this, "Error", err + ": " + name);
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
        addMacro(macro.name, macro.description, macro.hotkey);

        Logger::instance().mInfo("Macro created successfully: " + name);
    } else {
        Logger::instance().mError(err);
    }
}

