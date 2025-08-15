#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "languagemanager.h"
#include "logger.h"
#include "settingsmanager.h"
#include "appdatamanager.h"
#include "macromanager.h"
#include "settingswin.h"


#include <QJsonObject>
#include <QResizeEvent>
#include <QTimer>
#include <QMessageBox>

MainWindow::MainWindow(const QJsonObject& settings, const QVector<Macro>& macros, QWidget *parent)
    : QMainWindow(parent)
    , m_settings(settings)
    , m_macros(macros)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, &MainWindow::retranslateUi);

    QTimer::singleShot(0, this, &MainWindow::adjustTableColumns);
    ui->actionsTable->verticalHeader()->setVisible(false);

    // makro ayarlamaları
    int activeMacro = getSetting("ActiveMacro").toInt(1);
    Logger::instance().sInfo("(MainWindow) ActiveMacro from getSetting: " + QString::number(activeMacro));

    auto macroOpt = MacroManager::instance().getMacroById(activeMacro);
    if (macroOpt.has_value()) {
        QString activeMacroName = macroOpt->name;
        ui->labelActiveMacro->setText(tr("active macro label").replace("#MCR", activeMacroName));
        QVector<MacroAction> actions = MacroManager::instance().getActions(activeMacro);
        for (int i = 0; i < actions.length(); i++) {
            MacroAction act = actions.at(i);
            addAction(act.order, actionTypeToStr(act.action_type), "yarak");
        }
        Logger::instance().sInfo("(MainWindow) Active macro set to: " + activeMacroName + " (ID: " + QString::number(activeMacro) + ")");
    } else {
        Logger::instance().sError("(MainWindow) Active macro not found with ID: " + QString::number(activeMacro));
        // Fallback to first macro or default
        if (!m_macros.isEmpty()) {
            ui->labelActiveMacro->setText(tr("active macro label").replace("#MCR", m_macros.first().name));
        }
    }
}


QJsonValue MainWindow::getSetting(const QString& key) const
{
    return m_settings.value(key);
}

void MainWindow::setSetting(const QString& key, const QJsonValue& value)
{
    m_settings[key] = value;
    // Dosyaya da kaydet
    QString settingsPath = AppDataManager::instance().settingsFilePath();
    SettingsManager::instance().saveSettings(settingsPath, m_settings);
}

void MainWindow::saveActions(int macroId, const QVector<MacroAction>& actions) {
    QString err;
    if (!MacroManager::instance().setActionsForMacro(macroId, actions, &err)) {
        Logger::instance().mError(err);
        QMessageBox::warning(this, "Error", err);
    }
}

void MainWindow::addAction(int order, const QString& action_type, const QString& adds) {
    int row = ui->actionsTable->rowCount();
    ui->actionsTable->insertRow(row);

    ui->actionsTable->setItem(row, 0, new QTableWidgetItem(QString::number(order)));
    ui->actionsTable->setItem(row, 1, new QTableWidgetItem(action_type));
    ui->actionsTable->setItem(row, 2, new QTableWidgetItem(adds));
}


void MainWindow::retranslateUi()
{
    ui->retranslateUi(this);
}

void MainWindow::adjustTableColumns()
{
    int tableWidth = ui->actionsTable->viewport()->width();

    ui->actionsTable->setColumnWidth(0, tableWidth * 0.10); // Order
    ui->actionsTable->setColumnWidth(1, tableWidth * 0.40); // action type
    ui->actionsTable->setColumnWidth(2, tableWidth * 0.50); // additional
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

void MainWindow::on_actionSettings_triggered()
{
    // ÖNEMLİ: SettingsWin'e geçerli ayarları ver
    Logger::instance().sInfo("(MainWindow) Opening settings with ActiveMacro: " + QString::number(m_settings["ActiveMacro"].toInt()));

    SettingsWin settingsDialog(m_settings, this);
    int result = settingsDialog.exec();

    if (result == QDialog::Accepted) {
        // ÖNEMLİ: Settings dialog'dan güncellenmiş ayarları al
        QJsonObject updatedSettings = settingsDialog.m_settings;

        // Ayarları güncelle
        m_settings = updatedSettings;

        // Dosyaya kaydet
        QString settingsPath = AppDataManager::instance().settingsFilePath();
        SettingsManager::instance().saveSettings(settingsPath, m_settings);

        Logger::instance().sInfo("Settings updated successfully");
        Logger::instance().sInfo("New Language: " + m_settings["Language"].toString());
        Logger::instance().sInfo("New Theme: " + m_settings["Theme"].toString());
        Logger::instance().sInfo("Active Macro: " + QString::number(m_settings["ActiveMacro"].toInt()));

        // UI'ı güncelle (aktif makro label'ı vs)
        int activeMacro = m_settings["ActiveMacro"].toInt(1);
        auto macroOpt = MacroManager::instance().getMacroById(activeMacro);
        if (macroOpt.has_value()) {
            ui->labelActiveMacro->setText(tr("active macro label").replace("#MCR", macroOpt->name));
        }

    } else {
        Logger::instance().sInfo("Settings changes discarded");
    }
}
