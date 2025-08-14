#include "macroselectionwin.h"
#include "ui_macroselectionwin.h"

MacroSelectionWin::MacroSelectionWin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::MacroSelectionWin)
{
    ui->setupUi(this);
}

void MacroSelectionWin::adjustTableColumns()
{
    int tableWidth = ui->macrosTable->viewport()->width();

    ui->macrosTable->setColumnWidth(0, tableWidth * 0.10); // ID
    ui->macrosTable->setColumnWidth(1, tableWidth * 0.30); // NAME
    ui->macrosTable->setColumnWidth(2, tableWidth * 0.40); // DESCRIPTION
    ui->macrosTable->setColumnWidth(2, tableWidth * 0.20); // HOTKEY
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
