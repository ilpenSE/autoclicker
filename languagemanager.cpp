#include "languagemanager.h"

#include <QApplication>
#include <QDebug>
#include <QFile>

#include "Enums.h"
#include "logger.h"

QString LanguageManager::languageToLocale(Language lang) {
  return languageToLocaleMap.value(lang, "en_US");  // default en_US
}

Language LanguageManager::localeToLanguage(const QString &locale) {
  return localeToLanguageMap.value(locale, Language::ENGLISH);
}

bool LanguageManager::changeLanguage(const QString &languageCode) {
  QApplication::instance()->removeTranslator(&m_translator);

  bool loaded = m_translator.load(":/assets/locale/" + languageCode);
  if (loaded) {
    QApplication::instance()->installTranslator(&m_translator);
    m_currentLocale = languageCode;  // Mevcut locale'i kaydet
    emit languageChanged();
    Logger::instance().langInfo("Language changed to " + languageCode);
  } else {
    Logger::instance().langError("Failed to load language file: " +
                                 languageCode);
  }

  return loaded;
}

bool LanguageManager::changeLanguage(const Language &lang) {
  return changeLanguage(languageToLocale(lang));
}

bool LanguageManager::loadLanguage(const Language &lang) {
  return loadLanguage(languageToLocale(lang));
}

QString LanguageManager::getsyslang() {
  return QLocale::system()
      .name();  // sistem default dil kodunu döner (en_US, tr_TR gibi)
}

bool LanguageManager::loadLanguage(const QString &languageCode) {
  QString selectedLocale = languageCode;

  if (selectedLocale.isEmpty()) {
    // Eğer parametre verilmemişse sistem dillerinden dene
    const QString sysloc = getsyslang();
    Logger::instance().langInfo("[INFO] System locale: " + sysloc);

    QString qmPath = QString(":/assets/locale/%1.qm").arg(sysloc);
    selectedLocale = QFile::exists(qmPath) ? sysloc : "en_US";
  }

  // dili değiş
  return changeLanguage(selectedLocale);
}

QString LanguageManager::getCurrentLanguageStr() {
  // m_translator.language() güvenilir olmayabilir, kendi kaydettiğimizi kullan
  return m_currentLocale.isEmpty() ? "en_US" : m_currentLocale;
}

Language LanguageManager::getCurrentLanguage() {
  QString current = getCurrentLanguageStr();
  Language lang = localeToLanguage(current);

  // Debug için
  Logger::instance().langInfo(
      QString("getCurrentLanguage() - current locale: %1, mapped to enum: %2")
          .arg(current)
          .arg(static_cast<int>(lang)));

  return lang;
}
