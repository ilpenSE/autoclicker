#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "languagemanager.h"
#include "logger.h"
#include "settingsmanager.h"
#include "appdatamanager.h"

#include <QJsonObject>

bool supressLangChange = false;

MainWindow::MainWindow(const QJsonObject& settings, QWidget *parent)
    : QMainWindow(parent)
    , m_settings(settings)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, &MainWindow::retranslateUi);

    // LANGUAGE KULLANIMI: ui->testlabel->setText(tr("Hello World"));

    // langbox ayarlaması
    supressLangChange = true;
    QStringList langs;
    langs << "English";
    langs << "Türkçe";
    langs << "Deutsch";
    langs << "Français";
    langs << "Italiano";
    ui->langBox->addItems(langs);
    ui->langBox->setCurrentIndex(static_cast<int>(LanguageManager::instance().getCurrentLanguage()));
    supressLangChange = false;
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

void MainWindow::retranslateUi()
{
    ui->retranslateUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_langBox_currentIndexChanged(int index)
{
    if (!supressLangChange) {
        LanguageManager::instance().changeLanguage(static_cast<Language>(index));
        updateSetting("Language", LanguageManager::instance().getCurrentLanguageStr());

        Logger::instance().logInfo(LanguageManager::instance().getCurrentLanguageStr());
    }
}

