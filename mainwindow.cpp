#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "languagemanager.h"
#include "logger.h"
#include "settingsmanager.h"
#include "appdatamanager.h"
#include "macromanager.h"
#include "settingswin.h"
#include "thememanager.h"

#include <QJsonObject>
#include <QResizeEvent>
#include <QTimer>
#include <QMessageBox>
#include <QtSvg/QSvgRenderer>
#include <QPainter>

MainWindow::MainWindow(const QJsonObject& settings, const QVector<Macro>& macros, QWidget *parent)
    : QMainWindow(parent)
    , m_settings(settings)
    , m_macros(macros)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, &MainWindow::retranslateUi);

    // Connect to theme changes
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &MainWindow::onThemeChanged);

    QTimer::singleShot(0, this, &MainWindow::adjustTableColumns);
    ui->actionsTable->verticalHeader()->setVisible(false);

    // Setup dynamic icons after UI is ready
    QTimer::singleShot(0, this, &MainWindow::setupDynamicIcons);

    // makro ayarlamaları
    int activeMacro = getSetting("ActiveMacro").toInt(1);
    auto macroOpt = MacroManager::instance().getMacroById(activeMacro);
    if (macroOpt.has_value()) {
        // aktif makroyu çek ve ayarla
        QString activeMacroName = macroOpt->name;
        ui->labelActiveMacro->setText(tr("active macro label").replace("#MCR", activeMacroName));

        // aksiyon ekleme
        QVector<MacroAction> actions = MacroManager::instance().getActions(activeMacro);
        for (int i = 0; i < actions.length(); i++) {
            MacroAction act = actions.at(i);
            addActionToTable(act);
        }
        Logger::instance().sInfo("(MainWindow) Active macro set to: " + activeMacroName + " (ID: " + QString::number(activeMacro) + ")");
    } else {
        Logger::instance().sError("(MainWindow) Active macro not found with ID: " + QString::number(activeMacro));
        // Fallback to first macro or default
        if (!m_macros.isEmpty()) {
            ui->labelActiveMacro->setText(tr("active macro label").replace("#MCR", m_macros.first().name));
        }
    }

    loadLanguage();
}

void MainWindow::setupDynamicIcons() {
    // Use assets/icons from the project directory
    QString iconsPath = ":/assets/icons"; // Resource path

    // Setup QActions with dynamic icons
    // settings
    if (ui->actionSettings) {
        ThemeManager::instance().setupDynamicAction(
            ui->actionSettings,
            iconsPath + "/settings.svg",
            QSize(24, 24)
            );
    }
    // save
    if (ui->actionSave) {
        ThemeManager::instance().setupDynamicAction(
            ui->actionSave,
            iconsPath + "/save.svg",
            QSize(24, 24)
            );
    }
    // about
    if (ui->actionAbout) {
        ThemeManager::instance().setupDynamicAction(
            ui->actionAbout,
            iconsPath + "/info.svg",
            QSize(24, 24)
            );
    }

    Logger::instance().sInfo("Dynamic icons setup completed");
}

void MainWindow::onThemeChanged() {
    // This slot is called when theme changes
    // Icons are automatically updated by ThemeManager
    Logger::instance().sInfo("Theme changed, icons updated automatically");

    // You can add additional theme-related updates here if needed
    refreshIcons();
}

void MainWindow::refreshIcons() {
    // Force refresh all icons if needed
    ThemeManager::instance().refreshAllIcons();
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

void MainWindow::addActionToTable(MacroAction a) {
    // row ayarlaması
    int row = ui->actionsTable->rowCount();
    ui->actionsTable->insertRow(row);
    ui->actionsTable->setItem(row, 0, new QTableWidgetItem(QString::number(a.order)));

    // action type itemi eklemesi
    QString action_type_str = actionTypeToStr(a.action_type);
    QTableWidgetItem *atype = new QTableWidgetItem();
    atype->setData(Qt::UserRole, action_type_str);
    atype->setText(tr(qPrintable("atype " + action_type_str.toLower())));
    ui->actionsTable->setItem(row, 1, atype);

    // click type itemi ekleme
    QString click_type_str = clickTypeToStr(a.click_type);
    QTableWidgetItem *ctype = new QTableWidgetItem();
    ctype->setData(Qt::UserRole, click_type_str);
    ctype->setText(tr(qPrintable("ctype " + click_type_str.toLower())));
    ui->actionsTable->setItem(row, 2, ctype);

    // mouse btn itemi ekleme
    QString msbtnstr = a.mouse_button.has_value() ? mouseButtonToStr(a.mouse_button.value()) : "-";
    QTableWidgetItem *msbtn = new QTableWidgetItem();
    msbtn->setData(Qt::UserRole, msbtnstr); // - = NULL
    msbtn->setText(tr(qPrintable("msbtn " + msbtnstr.toLower())));
    ui->actionsTable->setItem(row, 3, msbtn);

    ui->actionsTable->setItem(row, 4, new QTableWidgetItem(QString::number(a.repeat)));
    ui->actionsTable->setItem(row, 5, new QTableWidgetItem(QString::number(a.interval)));
    ui->actionsTable->setItem(row, 6, new QTableWidgetItem("Adds..."));
}

void MainWindow::loadLanguage() {
    // ACTIONS TABLOSU ÇEVİRİLERİ

    // itemler
    for (int i = 0; i < ui->actionsTable->rowCount(); i++) {
        QTableWidgetItem* atype = ui->actionsTable->item(i, 1);
        QTableWidgetItem* ctype = ui->actionsTable->item(i, 2);
        QTableWidgetItem* msbtn = ui->actionsTable->item(i, 3);

        QString atype_str = atype->data(Qt::UserRole).toString().toLower();
        QString ctype_str = ctype->data(Qt::UserRole).toString().toLower();
        QString msbtn_str = msbtn->data(Qt::UserRole).toString().toLower();

        atype->setText(tr(qPrintable("atype " + atype_str)));
        ctype->setText(tr(qPrintable("ctype " + ctype_str)));
        msbtn->setText(tr(qPrintable("msbtn " + msbtn_str)));
    }

    // tooltipler
    ui->actionsTable->horizontalHeaderItem(0)->setToolTip("No");
    ui->actionsTable->horizontalHeaderItem(1)->setToolTip(tr("action type tooltip"));
    ui->actionsTable->horizontalHeaderItem(2)->setToolTip(tr("click type tooltip"));
    ui->actionsTable->horizontalHeaderItem(3)->setToolTip(tr("mouse button tooltip"));
    ui->actionsTable->horizontalHeaderItem(4)->setToolTip(tr("repeat tooltip"));
    ui->actionsTable->horizontalHeaderItem(5)->setToolTip(tr("interval tooltip"));
    ui->actionsTable->horizontalHeaderItem(6)->setToolTip(tr("additionals tooltip"));

    // headerlar
    ui->actionsTable->setHorizontalHeaderLabels(QStringList()
                                                << "No"
                                                << tr("action type")
                                                << tr("click type")
                                                << tr("mouse button")
                                                << tr("repeat")
                                                << tr("interval")
                                                << tr("additionals"));
}

void MainWindow::retranslateUi()
{
    ui->retranslateUi(this);
    loadLanguage();
}

void MainWindow::adjustTableColumns()
{
    int tableWidth = ui->actionsTable->viewport()->width();

    float noPerc = 0.05f;
    float actionTypePerc = 0.15f;
    float clickTypePerc = 0.15f;
    float msbtnPerc = 0.15f;
    float repeatPerc = 0.08f;
    float intervalPerc = 0.08f;
    float additionalPerc = 1.0f - (noPerc + actionTypePerc + clickTypePerc + repeatPerc + msbtnPerc + intervalPerc);

    ui->actionsTable->setColumnWidth(0, tableWidth * noPerc); // Order
    ui->actionsTable->setColumnWidth(1, tableWidth * actionTypePerc); // action type
    ui->actionsTable->setColumnWidth(2, tableWidth * clickTypePerc); // click type
    ui->actionsTable->setColumnWidth(3, tableWidth * msbtnPerc); // Mouse button
    ui->actionsTable->setColumnWidth(4, tableWidth * repeatPerc); // repeat
    ui->actionsTable->setColumnWidth(5, tableWidth * intervalPerc); // interval
    ui->actionsTable->setColumnWidth(6, tableWidth * additionalPerc); // additional
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
