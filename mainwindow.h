#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QJsonObject>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QJsonObject& settings, QWidget *parent = nullptr);

    void updateSetting(const QString& key, const QJsonValue& value);
    QJsonValue getSetting(const QString& key) const;

    ~MainWindow();

private slots:
    void addItemToMacros(const QString& name, const QString& desc, const QString& htk);
    void adjustTableColumns();
    void retranslateUi();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QJsonObject m_settings;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
