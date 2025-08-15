#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "languagemanager.h"
#include "logger.h"
#include "settingsmanager.h"
#include "appdatamanager.h"
#include "thememanager.h"
#include "macromanager.h"

#include <QJsonObject>
#include <QResizeEvent>
#include <QTimer>
#include <QMessageBox>

bool suppressLangBoxChange = true;

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

    // langbox ayarlaması
    /*
    QStringList langs;
    langs << "English";
    langs << "Türkçe";
    langs << "Deutsch";
    langs << "Français";
    langs << "Italiano";
    ui->langBox->addItems(langs);
    ui->langBox->setCurrentIndex(static_cast<int>(LanguageManager::instance().getCurrentLanguage()));
    suppressLangBoxChange = false;
    */

    // themebox ayarlaması
    /*
    ui->themeBox->clear();
    QString visname = ThemeManager::instance().getVisibleName(getSetting("Theme").toString());
    ui->themeBox->addItems(ThemeManager::instance().availableThemes());
    ui->themeBox->setCurrentText(visname);
    */

    int activeMacro = getSetting("ActiveMacro").toInt(1);
    QString activeMacroName = MacroManager::instance().getMacroById(activeMacro)->name;

    ui->labelActiveMacro->setText("Aktif Makro: " + activeMacroName + " (Değiştirmek için dokunun)");

    QVector<MacroAction> actions = MacroManager::instance().getActions(activeMacro);

    for (int i = 0; i < actions.length(); i++) {
        MacroAction act = actions.at(i);
        addAction(act.order, actionTypeToStr(act.action_type), "yarak");
    }
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

/*
void createNewMacro() {
    QString err;
    const int id = MacroManager::instance().createMacro("Yeni Makro 1", "Açıklama", "DEF", &err);
    if (id < 0) QMessageBox::warning(this, "Hata", err);
} */

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

