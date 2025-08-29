#ifndef ADDITIONALSETTINGSDIALOG_H
#define ADDITIONALSETTINGSDIALOG_H

#include <QDialog>

#include "macromanager.h"

namespace Ui {
class AdditionalSettingsDialog;
}

class AdditionalSettingsDialog : public QDialog {
  Q_OBJECT

 public:
  explicit AdditionalSettingsDialog(const MacroAction& action,
                                    QWidget* parent = nullptr);
  ~AdditionalSettingsDialog();

  MacroAction getUpdatedAction();

 private slots:
  void onCurrentPosToggled(bool enabled);
  void loadLanguage();

 private:
  Ui::AdditionalSettingsDialog* ui;
  MacroAction m_action;

  QString trans(QString str);

  void setupUI();
  void loadActionData();
  void updateActionData();
  void parsePosition(const QString& positionStr, int& x, int& y);
  QString formatPosition(int x, int y);
};

#endif  // ADDITIONALSETTINGSDIALOG_H
