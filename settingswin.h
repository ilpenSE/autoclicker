#ifndef SETTINGSWIN_H
#define SETTINGSWIN_H

#include <QDialog>

namespace Ui {
class SettingsWin;
}

class SettingsWin : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWin(QWidget *parent = nullptr);
    ~SettingsWin();

private:
    Ui::SettingsWin *ui;
};

#endif // SETTINGSWIN_H
