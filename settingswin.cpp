#include "settingswin.h"
#include "ui_settingswin.h"

SettingsWin::SettingsWin(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsWin)
{
    ui->setupUi(this);
}

SettingsWin::~SettingsWin()
{
    delete ui;
}
