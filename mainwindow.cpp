#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "languagemanager.h"
#include "logger.h"
#include "settingsmanager.h"
#include "appdatamanager.h"

#include <QJsonObject>
#include <QResizeEvent>
#include <QTimer>

bool suppressLangBoxChange = true;

MainWindow::MainWindow(const QJsonObject& settings, QWidget *parent)
    : QMainWindow(parent)
    , m_settings(settings)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, &MainWindow::retranslateUi);

    QTimer::singleShot(0, this, &MainWindow::adjustTableColumns);

    // langbox ayarlaması
    ui->langBox->setCurrentIndex(static_cast<int>(LanguageManager::instance().getCurrentLanguage()));
    suppressLangBoxChange = false;

    addItemToMacros("salla hacı", "bu bir açıklamadır", "F6");
}

void MainWindow::updateSetting(const QString& key, const QJsonValue& value)
{
    m_settings[key] = value;

    QString settingsPath = AppDataManager::instance().settingsFilePath();
    SettingsManager::instance().saveSettings(settingsPath, m_settings);
}

QJsonValue MainWindow::getSetting(const QString& key) const
{
    return m_settings.value(key);
}

void MainWindow::addItemToMacros(const QString& name, const QString& desc, const QString& htk) {
    int row = ui->tableMacros->rowCount();
    ui->tableMacros->insertRow(row);

    ui->tableMacros->setItem(row, 0, new QTableWidgetItem(QString::number(row + 1)));
    ui->tableMacros->setItem(row, 1, new QTableWidgetItem(name));
    ui->tableMacros->setItem(row, 2, new QTableWidgetItem(desc));
    ui->tableMacros->setItem(row, 3, new QTableWidgetItem(htk));

}

void MainWindow::retranslateUi()
{
    ui->retranslateUi(this);
}
void MainWindow::adjustTableColumns()
{
    int tableWidth = ui->tableMacros->viewport()->width();

    ui->tableMacros->setColumnWidth(0, tableWidth * 0.10); // No
    ui->tableMacros->setColumnWidth(1, tableWidth * 0.30); // İsim
    ui->tableMacros->setColumnWidth(2, tableWidth * 0.40); // Açıklama
    ui->tableMacros->setColumnWidth(3, tableWidth * 0.20); // Kısayol
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    adjustTableColumns();
}

MainWindow::~MainWindow()
{
    Logger::instance().logInfo("Closing the app...");
    delete ui;
}

/*
void MainWindow::on_langBox_currentIndexChanged(int index)
{
    if (!suppressLangBoxChange) {
        qDebug() << "index: " << index;
        LanguageManager::instance().changeLanguage(static_cast<Language>(index));
        updateSetting("Language", LanguageManager::instance().getCurrentLanguageStr());

        Logger::instance().logInfo(LanguageManager::instance().getCurrentLanguageStr());
    }
}
*/

