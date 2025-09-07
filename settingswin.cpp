#include "settingswin.h"

#include <QMessageBox>
#include <QTimer>

#include "LoggerStream.h"
#include "languagemanager.h"
#include "logger.h"
#include "macromanager.h"
#include "thememanager.h"
#include "ui_settingswin.h"
#include "consts.h"
#include "instances.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QEventLoop>
#include <QTextEdit>
#include <QUrl>
#include <QDebug>

SettingsWin::SettingsWin(const QJsonObject& settings, QWidget* parent)
    : QDialog(parent), m_settings(settings), ui(new Ui::SettingsWin) {
  ui->setupUi(this);
  connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this,
          &SettingsWin::retranslateUi);

  connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this,
          &SettingsWin::onThemeChanged);

  // Hotkey widget bağlantıları
  connect(ui->lineHotkey, &HotkeyLineEdit::hotkeyReady, this,
          &SettingsWin::onHotkeyReady);
  connect(ui->lineHotkey, &HotkeyLineEdit::hotkeyChanged, this,
          &SettingsWin::onHotkeyChanged);

  // Select hotkey butonu bağlantısı
  connect(ui->btnSelectHotkey, &QPushButton::clicked, this,
          &SettingsWin::on_btnSelectHotkey_clicked);

  QTimer::singleShot(0, this, &SettingsWin::setupDynamicIcons);

  // langbox ayarlaması
  QStringList langs;
  langs << "English";
  langs << "Türkçe";
  langs << "Deutsch";
  langs << "Français";
  langs << "Italiano";
  ui->langBox->addItems(langs);
  ui->langBox->setCurrentIndex(
      static_cast<int>(_langman().getCurrentLanguage()));

  // Mevcut dili doğru şekilde ayarla
  Language currentLang = _langman().getCurrentLanguage();
  ui->langBox->setCurrentIndex(static_cast<int>(currentLang));

  // theme box ayarlaması
  QString currentThemeFile = getSetting("Theme").toString();
  QString visibleThemeName =
      _themesman().getVisibleName(currentThemeFile);

  ui->themeBox->clear();
  QStringList availableThemes = _themesman().availableThemes();
  ui->themeBox->addItems(availableThemes);

  int themeIndex = ui->themeBox->findText(visibleThemeName);
  if (themeIndex >= 0) {
    ui->themeBox->setCurrentIndex(themeIndex);
  } else {
    thwrn() << "Theme not found in list: " + visibleThemeName;
    ui->themeBox->setCurrentIndex(0);
  }

  // hotkey box ayarlaması
  QString currentHotkey = getSetting("DefaultHotkey").toString();
  ui->lineHotkey->setHotkey(currentHotkey);

  QTimer::singleShot(1, this, [this]() {
    loadLanguage();
    loadPatchNotes();
  });
}

void SettingsWin::setupDynamicIcons() {
  // Use assets/icons from the project directory
  QString iconsPath = ":/assets/icons";  // Resource path

  // Setup QActions with dynamic icons
  // save
  if (ui->btnSave) {
    _themesman().setupDynamicButton(
        ui->btnSave, iconsPath + "/save.svg", QSize(24, 24));
  }
  // discard
  if (ui->btnDiscard) {
    _themesman().setupDynamicButton(
        ui->btnDiscard, iconsPath + "/cancel.svg", QSize(24, 24));
  }
  // select hotkey icon btn
  if (ui->btnSelectHotkey) {
    ui->btnSelectHotkey->setProperty("iconOnly", true);
    _themesman().setupDynamicButton(
        ui->btnSelectHotkey, iconsPath + "/select.svg", QSize(24, 24));
  }

  thinfo() << "Dynamic icons setup completed";
}

void SettingsWin::onThemeChanged() { refreshIcons(); }

void SettingsWin::refreshIcons() { _themesman().refreshAllIcons(); }

void SettingsWin::onHotkeyReady(const QString& hotkey) {
  hsinfo() << "Hotkey ready: " + hotkey;
}

void SettingsWin::onHotkeyChanged() {
  hsinfo() << "Hotkey changed to: " + ui->lineHotkey->getHotkey();
}

void SettingsWin::on_btnSelectHotkey_clicked() {
  if (ui->lineHotkey->isCapturing()) {
    // Capture modundaysa, hotkey'i tamamla
    ui->lineHotkey->stopCapture();
    hsinfo() << "Hotkey capture completed: " + ui->lineHotkey->getHotkey();
  }
  ui->lineHotkey->clearFocus();  // fokus kalksın
}

QString SettingsWin::trans(const QString& key) {
  return QApplication::translate("MainWindow", qPrintable(key));
}

void SettingsWin::loadLanguage() {
  this->setWindowTitle(trans("settings"));

  ui->labelLanguage->setText(trans("language"));
  ui->labelHotkey->setText(trans("general hotkey"));
  ui->labelPatchNotes->setText(trans("patch notes"));
  ui->labelTheme->setText(trans("theme"));

  ui->btnSave->setText(trans("save"));
  ui->btnDiscard->setText(trans("cancel"));
}

void SettingsWin::retranslateUi() {
  ui->retranslateUi(this);
  loadLanguage();
}

void SettingsWin::updateSetting(const QString& key, const QJsonValue& value) {
  m_settings[key] = value;
}

QJsonValue SettingsWin::getSetting(const QString& key) const {
  return m_settings.value(key);
}

SettingsWin::~SettingsWin() { delete ui; }

void SettingsWin::on_btnSave_clicked() {
  // Mevcut ActiveMacro değerini koru
  int currentActiveMacro = m_settings["ActiveMacro"].toInt();

  // language değerini kaydet
  Language selectedLang = static_cast<Language>(ui->langBox->currentIndex());
  QString selectedLocale =
      _langman().languageToLocale(selectedLang);

  _langman().changeLanguage(selectedLang);
  m_settings["Language"] = selectedLocale;

  // theme değerini kaydet
  QString themeVisibleName = ui->themeBox->currentText();
  QString themeFileName =
      _themesman().getFileName(themeVisibleName);
  if (_themesman().applyTheme(themeVisibleName)) {
    m_settings["Theme"] = themeFileName;
    thinfo() << "Theme changed to " << themeVisibleName << " (" << themeFileName
             << ".qss)";
  } else {
    therr() << "Theme cannot be changed to " + themeVisibleName +
                   ", file name: " + themeFileName;
  }

  // Hotkey değerini kaydet
  QString newHotkey = ui->lineHotkey->getHotkey();
  if (!newHotkey.isEmpty()) {
    QString error;
    if (!MacroManager::instance().validateHotkey(newHotkey, &error)) {
      serr() << error;
      QMessageBox::critical(this, trans("error"),
                            "No valid hotkey: " + newHotkey);
      return;
    }

    m_settings["DefaultHotkey"] = newHotkey;
    sinfo() << "Default hotkey changed to: " + newHotkey;
  }

  // ActiveMacro'yu geri yükle
  m_settings["ActiveMacro"] = currentActiveMacro;

  // Kaydet
  accept();
}

void SettingsWin::on_btnDiscard_clicked() {
  // CANCEL
  reject();
}
#include "appdatamanager.h"
void SettingsWin::loadPatchNotes() {
  // Markdown olarak QTextEdit'e yaz
  QString notes = _appdataman().getPatchNotes();
  ui->textPatchNotes->setMarkdown(notes == "Cannot load patch notes." ? trans(notes) : notes);
}
