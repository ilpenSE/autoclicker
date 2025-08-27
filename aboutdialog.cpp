#include "aboutdialog.h"

#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent)
    : QDialog(parent), ui(new Ui::AboutDialog) {
  ui->setupUi(this);

  ui->labelText->setTextFormat(Qt::RichText);
  ui->labelText->setTextInteractionFlags(Qt::TextBrowserInteraction);
  ui->labelText->setOpenExternalLinks(true);

  // Yazı
  QString txt = ui->labelText->text()
                    .replace("ABT", QApplication::translate("MainWindow", "about"))
                    .replace("VER", QApplication::translate("MainWindow", "about text version"))
                    .replace("ATHR", QApplication::translate("MainWindow", "about text author"));
  ui->labelText->setText(txt);

  this->setWindowTitle(QApplication::translate("MainWindow", "about"));

  connect(ui->buttonOK, &QPushButton::clicked, this, &QDialog::accept);
}

AboutDialog::~AboutDialog() { delete ui; }
