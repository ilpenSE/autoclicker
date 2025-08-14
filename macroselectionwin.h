#ifndef MACROSELECTIONWIN_H
#define MACROSELECTIONWIN_H

#include <QDialog>

namespace Ui {
class MacroSelectionWin;
}

class MacroSelectionWin : public QDialog
{
    Q_OBJECT

public:
    explicit MacroSelectionWin(QWidget *parent = nullptr);
    ~MacroSelectionWin();

private slots:
    void addMacro(const QString& name, const QString& desc, const QString& htk);
    void adjustTableColumns();
    void retranslateUi();

protected:
    void resizeEvent(QResizeEvent* event) override;

private:
    Ui::MacroSelectionWin *ui;
};

#endif // MACROSELECTIONWIN_H
