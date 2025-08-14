#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QVector>
#include "macromanager.h"
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
    explicit MainWindow(const QJsonObject& settings, const QVector<Macro>& macros, QWidget *parent = nullptr);

    void updateSetting(const QString& key, const QJsonValue& value);
    QJsonValue getSetting(const QString& key) const;

    ~MainWindow();

private slots:
    void addAction(int order, const QString& action_type, const QString& adds);
    void adjustTableColumns();
    void retranslateUi();
    void saveActions(int macroId, const QVector<MacroAction>& actions);

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    QJsonObject m_settings;
    QVector<Macro> m_macros;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
