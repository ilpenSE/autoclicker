#ifndef UPDATEDIALOG_H
#define UPDATEDIALOG_H

#include <QDialog>

namespace Ui {
class UpdateDialog;
}

class UpdateDialog : public QDialog
{
    Q_OBJECT

public:
    void updateProgress(qint64 received, qint64 total);

    explicit UpdateDialog(QWidget *parent = nullptr);
    ~UpdateDialog();

private:
    Ui::UpdateDialog *ui;
};

#endif // UPDATEDIALOG_H
