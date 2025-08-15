#ifndef SETTINGSWIN_H
#define SETTINGSWIN_H

#include <QDialog>
#include <QJsonObject>

namespace Ui {
class SettingsWin;
}

class SettingsWin : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWin(const QJsonObject& settings, QWidget *parent = nullptr);
    ~SettingsWin();
    QJsonObject m_settings;
    QJsonValue getSetting(const QString& key) const;

private slots:
    void retranslateUi();
    void updateSetting(const QString& key, const QJsonValue& value);
    void loadLanguage();
    void on_btnSave_clicked();

    void on_btnDiscard_clicked();

private:
    Ui::SettingsWin *ui;
};

#endif // SETTINGSWIN_H
