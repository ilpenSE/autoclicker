#include "settingswin.h"
#include "ui_settingswin.h"
#include "languagemanager.h"
#include "thememanager.h"
#include "logger.h"
#include "settingsmanager.h"
#include "appdatamanager.h"

SettingsWin::SettingsWin(const QJsonObject& settings, QWidget *parent)
    : QDialog(parent)
    , m_settings(settings)
    , ui(new Ui::SettingsWin)
{
    ui->setupUi(this);
    connect(&LanguageManager::instance(), &LanguageManager::languageChanged,
            this, &SettingsWin::retranslateUi);

    // langbox ayarlaması
    QStringList langs;
    langs << "English";
    langs << "Türkçe";
    langs << "Deutsch";
    langs << "Français";
    langs << "Italiano";
    ui->langBox->addItems(langs);
    ui->langBox->setCurrentIndex(static_cast<int>(LanguageManager::instance().getCurrentLanguage()));

    // Mevcut dili doğru şekilde ayarla
    Language currentLang = LanguageManager::instance().getCurrentLanguage();
    ui->langBox->setCurrentIndex(static_cast<int>(currentLang));

    // Debug için
    Logger::instance().langInfo(QString("Current language enum: %1, index: %2")
                                    .arg(static_cast<int>(currentLang))
                                    .arg(ui->langBox->currentIndex()));

    // theme box ayarlaması
    QString currentThemeFile = getSetting("Theme").toString();
    QString visibleThemeName = ThemeManager::instance().getVisibleName(currentThemeFile);

    ui->themeBox->clear();
    QStringList availableThemes = ThemeManager::instance().availableThemes();
    ui->themeBox->addItems(availableThemes);

    int themeIndex = ui->themeBox->findText(visibleThemeName);
    if (themeIndex >= 0) {
        ui->themeBox->setCurrentIndex(themeIndex);
    } else {
        Logger::instance().thWarning("Theme not found in list: " + visibleThemeName);
        ui->themeBox->setCurrentIndex(0);
    }

    retranslateUi();
}

void SettingsWin::loadLanguage() {
    // MAIN WINDOW HARİCİ KULLANILAN TRANSLATELERDE BÖYLE YAP YOKSA BU CLASSI ALIR
    ui->btnSave->setText(QApplication::translate("MainWindow", "save"));
    ui->btnDiscard->setText(QApplication::translate("MainWindow", "cancel"));
}

void SettingsWin::retranslateUi()
{
    ui->retranslateUi(this);
    loadLanguage();
}

void SettingsWin::updateSetting(const QString& key, const QJsonValue& value)
{
    m_settings[key] = value;

    QString settingsPath = AppDataManager::instance().settingsFilePath();
    SettingsManager::instance().saveSettings(settingsPath, m_settings);
}

QJsonValue SettingsWin::getSetting(const QString& key) const
{
    return m_settings.value(key);
}

SettingsWin::~SettingsWin()
{
    delete ui;
}

void SettingsWin::on_btnSave_clicked()
{
    // ÖNEMLI: Mevcut ActiveMacro değerini koru
    int currentActiveMacro = m_settings["ActiveMacro"].toInt();

    // language değerini kaydet
    Language selectedLang = static_cast<Language>(ui->langBox->currentIndex());
    QString selectedLocale = LanguageManager::instance().languageToLocale(selectedLang);

    // Debug için
    Logger::instance().langInfo(QString("Selected language index: %1, enum: %2, locale: %3")
                                    .arg(ui->langBox->currentIndex())
                                    .arg(static_cast<int>(selectedLang))
                                    .arg(selectedLocale));

    LanguageManager::instance().changeLanguage(selectedLang);
    m_settings["Language"] = selectedLocale;

    // theme değerini kaydet
    QString themeVisibleName = ui->themeBox->currentText();
    QString themeFileName = ThemeManager::instance().getFileName(themeVisibleName);
    if (ThemeManager::instance().applyTheme(themeVisibleName)) {
        m_settings["Theme"] = themeFileName;
        Logger::instance().thInfo("Theme changed to " + themeVisibleName + " (" + themeFileName + ".qss)");
    }
    else {
        Logger::instance().thError("Theme cannot be changed to " + themeVisibleName + ", file name: " + themeFileName);
    }

    // ÖNEMLI: ActiveMacro'yu geri yükle
    m_settings["ActiveMacro"] = currentActiveMacro;

    // Debug için
    Logger::instance().sInfo(QString("Saving settings - Language: %1, Theme: %2, ActiveMacro: %3")
                                 .arg(selectedLocale)
                                 .arg(themeFileName)
                                 .arg(currentActiveMacro));

    // Kaydet
    SettingsManager::instance().saveSettings(AppDataManager::instance().settingsFilePath(), m_settings);
    accept();
}

void SettingsWin::on_btnDiscard_clicked()
{
    // CANCEL
    reject();
}

