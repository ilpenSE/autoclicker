#ifndef MACROSELECTIONWIN_H
#define MACROSELECTIONWIN_H

#include <QDialog>
#include "macromanager.h"

namespace Ui {
class MacroSelectionWin;
}

class MacroSelectionWin : public QDialog
{
    Q_OBJECT

public:
    QVector<Macro>& m_macros;
    int activeMacroId;

    explicit MacroSelectionWin(QVector<Macro>& macros, QWidget *parent = nullptr);
    ~MacroSelectionWin();

private slots:
    void addMacro(const QString& name, const QString& desc, const QString& htk);
    void adjustTableColumns();
    void retranslateUi();
    void setupDynamicIcons();
    void refreshIcons();
    void onThemeChanged();

    void on_btnSelect_clicked();

    void on_btnCancel_clicked();

    void on_btnDelete_clicked();

    void on_btnCreate_clicked();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    void loadLang();
    QString trans(const QString& key);
    Ui::MacroSelectionWin *ui;
};

#endif // MACROSELECTIONWIN_H
