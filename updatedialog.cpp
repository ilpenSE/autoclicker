#include "updatedialog.h"
#include "ui_updatedialog.h"

UpdateDialog::UpdateDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UpdateDialog)
{
    ui->setupUi(this);
}
void UpdateDialog::updateProgress(qint64 received, qint64 total) {
    if (total > 0) {
        int percent = int((received * 100) / total);
        ui->progressBar->setValue(percent);
        ui->labelStatus->setText(QString("Downloading... %1%").arg(percent));
    }
}
UpdateDialog::~UpdateDialog()
{
    delete ui;
}
