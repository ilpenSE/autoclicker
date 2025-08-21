#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "languagemanager.h"
#include "logger.h"
#include "settingsmanager.h"
#include "appdatamanager.h"
#include "macromanager.h"
#include "settingswin.h"
#include "thememanager.h"
#include "macroselectionwin.h"
#include "hotkeyservice.h"
#include "LoggerStream.h"

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

    // clicker ve hotkey ayarlamaları
    clickEngine = ClickEngine::getInstance();
    hotkeyService = HotkeyService::getInstance();

    activeMacro = MacroManager::instance().getMacroById(getSetting("ActiveMacro").toInt(1)).value();

    // hotkey registeration
    bool isDefKey = activeMacro.hotkey == "DEF";
    QString htkstr = isDefKey ? getSetting("DefaultHotkey").toString() : activeMacro.hotkey;
    if (!hotkeyService->registerHotkey(htkstr, activeMacro.id)) {
        hserr() << "Hotkey registeration failed!";
        return;
    }

    // clicker engine start/stop eventleri
    connect(clickEngine, &ClickEngine::macroStarted, this, &MainWindow::onMacroStarted);
    connect(clickEngine, &ClickEngine::macroStopped, this, &MainWindow::onMacroStopped);

    // hotkey pressed eventleri
    connect(hotkeyService, &HotkeyService::hotkeyPressed, this,
            [this](int id) {
                hsinfo() << "Hotkey pressed!";
                if (clickEngine->isMacroRunning() && clickEngine->getCurrentMacroId() == id) {
                    clickEngine->stopCurrentMacro();
                } else {
                    clickEngine->startMacro(id);
                }
            }
            );

    connect(hotkeyService, &HotkeyService::hotkeyRegistrationFailed, this,
            [](int id, const QString& reason) {
        hserr() << "Hotkey registration failed, reason: " << reason << ", id: " << id;
    });

    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, &MainWindow::retranslateUi);

    // Connect to theme changes
    connect(&ThemeManager::instance(), &ThemeManager::themeChanged,
            this, &MainWindow::onThemeChanged);

    // tablo ui ayarlamaları
    QTimer::singleShot(0, this, &MainWindow::adjustTableColumns);
    ui->actionsTable->verticalHeader()->setVisible(false);

    // Setup dynamic icons after UI is ready
    QTimer::singleShot(0, this, &MainWindow::setupDynamicIcons);

    // makro ayarlamaları
    setActiveMacro(getSetting("ActiveMacro").toInt(1));

    loadLanguage();

    ui->labelErrors->setVisible(false);
}

void MainWindow::onMacroStarted(int id) {
    minfo() << "Macro started: " << id;

    ui->btnStart->setEnabled(false);
    ui->btnStop->setEnabled(true);
}

void MainWindow::onMacroStopped(int id) {
    minfo() << "Macro stopped: " << id;
    ui->btnStart->setEnabled(true);
    ui->btnStop->setEnabled(false);
}

void MainWindow::onMacroError(int id, const QString &error) {
    merr() << "Macro error in " << id << ": " << error;
}


void MainWindow::setActiveMacro(int id) {
    auto macro = MacroManager::instance().getMacroById(id);
    if (macro.has_value()) {
        QString activeMacroName = macro->name;
        ui->actionActiveMacro->setText(tr("active macro label").replace("#MCR", activeMacroName));
        activeMacro = macro.value();
        sinfo() << "(MainWindow) Active macro set to: " << activeMacroName << " (ID: " << id << ")";
    } else {
        serr() << "(MainWindow) Active macro not found with ID: " << id;
        // Fallback to first macro or default
        if (!m_macros.isEmpty()) {
            ui->actionActiveMacro->setText(tr("active macro label").replace("#MCR", m_macros.first().name));
            id = m_macros.first().id;
            activeMacro = MacroManager::instance().getMacroById(1).value();
        }
    }

    ui->actionsTable->setRowCount(0);
    QVector<MacroAction> acts = MacroManager::instance().getActions(id);
    for (const MacroAction& act : acts) {
        addActionToTable(act);
    }
}

void MainWindow::setupDynamicIcons() {
    // Use assets/icons from the project directory
    QString iconsPath = ":/assets/icons"; // Resource path

    // Setup QActions with dynamic icons
    // active macro
    if (ui->actionActiveMacro) {
        ThemeManager::instance().setupDynamicAction(
            ui->actionActiveMacro,
            iconsPath + "/select.svg",
            QSize(24, 24)
            );
    }
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

    thinfo() << "Dynamic icons setup completed";
}

void MainWindow::onThemeChanged() {
    // This slot is called when theme changes
    // Icons are automatically updated by ThemeManager
    thinfo() << "Theme changed, icons updated automatically";

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
}

void MainWindow::saveActions(int macroId, const QVector<MacroAction>& actions) {
    QString err;
    if (!MacroManager::instance().setActionsForMacro(macroId, actions, &err)) {
        merr() << err;
        QMessageBox::critical(this, tr("error"), err);
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
    atype->setText(tr(qPrintable((action_type_str == "-" ? "" : "atype ") + action_type_str.toLower())));
    ui->actionsTable->setItem(row, 1, atype);

    // click type itemi ekleme
    QString click_type_str = clickTypeToStr(a.click_type);
    QTableWidgetItem *ctype = new QTableWidgetItem();
    ctype->setData(Qt::UserRole, click_type_str);
    ctype->setText(tr(qPrintable((click_type_str == "-" ? "" :"ctype ") + click_type_str.toLower())));
    ui->actionsTable->setItem(row, 2, ctype);

    // mouse btn itemi ekleme
    QString msbtnstr = a.mouse_button.has_value() ? mouseButtonToStr(a.mouse_button.value()) : "-";
    QTableWidgetItem *msbtn = new QTableWidgetItem();
    msbtn->setData(Qt::UserRole, msbtnstr); // - = NULL
    msbtn->setText(tr(qPrintable((msbtnstr == "-" ? "" : "msbtn ") + msbtnstr.toLower())));
    ui->actionsTable->setItem(row, 3, msbtn);

    ui->actionsTable->setItem(row, 4, new QTableWidgetItem(QString::number(a.repeat)));
    ui->actionsTable->setItem(row, 5, new QTableWidgetItem(QString::number(a.interval)));
    ui->actionsTable->setItem(row, 6, new QTableWidgetItem("..."));
}

void MainWindow::loadLanguage() {
    // TOOLBAR ACTION ÇEVİRİLERİ
    ui->actionAbout->setText(tr("about"));
    ui->actionSettings->setText(tr("settings"));
    ui->actionSave->setText(tr("macro save"));

    // MACRO ACTIONS TABLOSU ÇEVİRİLERİ
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

    ui->btnStart->setToolTip(tr("start tooltip"));
    ui->btnStop->setToolTip(tr("stop tooltip"));

    ui->actionActiveMacro->setToolTip(tr("active macro label tooltip"));
    ui->actionSave->setToolTip(tr("macro save tooltip"));
    ui->actionSettings->setToolTip(tr("settings tooltip"));
    ui->actionAbout->setToolTip(tr("about tooltip"));

    ui->btnAddAction->setToolTip(tr("add action tooltip"));
    ui->btnDeleteAction->setToolTip(tr("delete action tooltip"));
    ui->btnEditAction->setToolTip(tr("edit orders tooltip"));

    // headerlar
    ui->actionsTable->setHorizontalHeaderLabels(QStringList()
                                                << "No"
                                                << tr("action type")
                                                << tr("click type")
                                                << tr("mouse button")
                                                << tr("repeat")
                                                << tr("interval")
                                                << tr("additionals"));

    // BUTONLAR
    ui->btnAddAction->setText(tr("add action"));
    ui->btnDeleteAction->setText(tr("delete action"));
    ui->btnEditAction->setText(tr("edit action orders"));

    QString htkStr = " (" + (activeMacro.hotkey == "DEF" ? getSetting("DefaultHotkey").toString() : activeMacro.hotkey) + ")";
    ui->btnStart->setText(tr("start") + htkStr);
    ui->btnStop->setText(tr("stop") + htkStr);
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
    float actionTypePerc = 0.20f;
    float clickTypePerc = 0.20f;
    float msbtnPerc = 0.20f;
    float repeatPerc = 0.13f;
    float intervalPerc = 0.13f;
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
    QString settingsPath = AppDataManager::instance().settingsFilePath();
    if (SettingsManager::instance().saveSettings(settingsPath, m_settings)) {
        sinfo() << "The settings file has been synchronized with the UI.";
    } else {
        serr() << "Failed to synchronize the settings file with the UI.”";
    }

    info() << "Closing the app...";
    delete ui;
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsWin settingsDialog(m_settings, this);
    int result = settingsDialog.exec();

    if (result == QDialog::Accepted) {
        // ÖNEMLİ: Settings dialog'dan güncellenmiş ayarları al
        QJsonObject updatedSettings = settingsDialog.m_settings;

        // Ayarları güncelle
        m_settings = updatedSettings;

        SettingsManager::instance().saveSettings(AppDataManager::instance().settingsFilePath(), updatedSettings);

        // UI güncellemeleri
        // aktif makro
        std::optional<Macro> query = MacroManager::instance().getMacroById(m_settings["ActiveMacro"].toInt(1));
        Macro activeMacro;
        if (query.has_value()) {
            activeMacro = query.value();
        }
        setActiveMacro(activeMacro.id);

        // hotkey
        bool isDefaultHotkey = activeMacro.hotkey == "DEF";
        QString htkstr = isDefaultHotkey ? m_settings["DefaultHotkey"].toString() : activeMacro.hotkey;
        QString startText = tr("start");
        QString stopText = tr("stop");
        QString appendedStr = " (" + htkstr + ")";

        if (!hotkeyService->changeHotkey(activeMacro.id, htkstr)) {
            hserr() << "Hotkey change failed after settings tab!";
            return;
        }

        startText += appendedStr;
        stopText += appendedStr;

        ui->btnStart->setText(startText);
        ui->btnStop->setText(stopText);
    }
}

void MainWindow::refreshMacros() {
    m_macros = MacroManager::instance().getAllMacros();
    Logger::instance().mInfo(QString("Macros refreshed. Count: %1").arg(m_macros.size()));
}

void MainWindow::on_actionActiveMacro_triggered()
{
    refreshMacros();

    MacroSelectionWin selectionDialog(m_macros, getSetting("ActiveMacro").toInt(1), this);
    int res = selectionDialog.exec();

    if (res == QDialog::Accepted) {
        // selectionDialog.activeMacroId artık gerçek SQL ID'si
        int newActiveMacroId = selectionDialog.activeMacroId;
        int oldMacroId = activeMacro.id;

        // Aktif makroyu ayarla
        setActiveMacro(newActiveMacroId);
        setSetting("ActiveMacro", newActiveMacroId);

        QString htkStr = activeMacro.hotkey == "DEF" ? getSetting("DefaultHotkey").toString() : activeMacro.hotkey;
        ui->btnStart->setText(tr("start") + " (" + htkStr + ")");
        ui->btnStop->setText(tr("stop") + " (" + htkStr + ")");

        // eski makronun hotkeyini unregister et
        if (!hotkeyService->unregisterHotkey(oldMacroId)) {
            Logger::instance().hsError("Old macro's hotkey unregisteration failed after macro changing!");
            return;
        }
        // yeni makronun hotkeyini register et
        if (!hotkeyService->registerHotkey(htkStr, newActiveMacroId)) {
            Logger::instance().hsError("New macro's hotkey registeration failed after macro changing!");
            return;
        }

        Logger::instance().mInfo("Active macro set to ID: " + QString::number(newActiveMacroId));
    }
}

