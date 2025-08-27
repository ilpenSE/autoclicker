#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QJsonObject>
#include <QMainWindow>
#include <QVector>

#include "clickengine.h"
#include "hotkeyservice.h"
#include "macromanager.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(const QJsonObject& settings, const QVector<Macro>& macros,
                      QWidget* parent = nullptr);

  ~MainWindow();

 private slots:
  void addActionToTable(MacroAction a);
  void adjustTableColumns();
  void retranslateUi();
  void saveActions(int macroId, const QVector<MacroAction>& actions);
  void setSetting(const QString& key, const QJsonValue& value);
  void on_actionSettings_triggered();
  void loadLanguage();
  void onThemeChanged();

  void onMacroStarted(int macroId);
  void onMacroStopped(int macroId);
  void onMacroError(int macroId, const QString& error);

  void on_actionActiveMacro_triggered();
  QString getHotkeyString(QString htkstr);

  void on_actionAbout_triggered();

 protected:
  void resizeEvent(QResizeEvent* event) override;

 private:
  QJsonObject m_settings;
  QVector<Macro> m_macros;
  ClickEngine* clickEngine;
  HotkeyService* hotkeyService;

  Macro activeMacro;
  Ui::MainWindow* ui;

  void refreshMacros();
  void setActiveMacro(int id);
  QJsonValue getSetting(const QString& key) const;
  void setupDynamicIcons();
  void refreshIcons();
};
#endif  // MAINWINDOW_H
