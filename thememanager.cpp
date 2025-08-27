#include "thememanager.h"

#include <QApplication>
#include <QIcon>
#include <QImage>
#include <QJsonObject>
#include <QPainter>
#include <QPointer>
#include <QRegularExpression>
#include <QStandardPaths>
#include <QtSvg/QSvgRenderer>

#include "LoggerStream.h"
#include "appdatamanager.h"
#include "logger.h"
// Existing global variables
QMap<QString, QString> themeMap;
QMap<QString, QString> reversedThemeMap;

ThemeManager::ThemeManager(QObject* parent) : QObject(parent) {
  loadThemesFromJson();
  reversedThemeMap = reverseThemeMap();
}

// Existing functions implementation...
QString ThemeManager::themesDirPath() const {
  QString path = AppDataManager::instance().appFolderPath();
  QDir dir(path + "/themes");
  if (!dir.exists()) {
    dir.mkpath(".");
    fswrn() << "Themes folder not found, creating one...";
  }
  return dir.absolutePath();
}

QString ThemeManager::themesJsonPath() const {
  QString path = AppDataManager::instance().appFolderPath();
  return path + "/themes.json";
}

QString ThemeManager::readQssFile(const QString& filePath) const {
  QFile file(filePath);
  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    fserr() << "Theme file cannot be opened: " + filePath;
    return QString();
  }
  return QString::fromUtf8(file.readAll());
}

void ThemeManager::loadThemesFromJson() {
  themeMap.clear();

  QFile file(themesJsonPath());
  if (!file.exists()) {
    fserr() << "themes.json not found: " + themesJsonPath();
    return;
  }

  if (!file.open(QFile::ReadOnly | QFile::Text)) {
    fserr() << "Cannot open themes.json: " + themesJsonPath();
    return;
  }

  QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
  if (!doc.isObject()) {
    fserr() << "themes.json is not a valid JSON object.";
    return;
  }

  QJsonObject obj = doc.object();
  for (auto it = obj.begin(); it != obj.end(); ++it) {
    QString qssFile = it.key();
    QString visibleName = it.value().toString();
    themeMap[visibleName] = qssFile;
    thinfo() << "Theme loaded, visible name: " + visibleName +
                    " / file name: " + qssFile;
  }

  thinfo() << "Loaded themes from themes.json";
}

QPixmap ThemeManager::coloredPixmap(const QString& svgPath, const QColor& color,
                                    const QSize& size) {
  if (!QFile::exists(svgPath) || size.isEmpty()) {
    thwrn() << "Pixmap is empty!";
    return QPixmap();  // boş pixmap
  }

  QSvgRenderer renderer(svgPath);
  if (!renderer.isValid()) {
    fserr() << "Invalid SVG: " + svgPath;
    return QPixmap();
  }

  QPixmap pix(size);
  pix.fill(Qt::transparent);

  {
    QPainter painter(&pix);
    painter.setRenderHint(QPainter::Antialiasing);

    // First render SVG
    renderer.render(&painter);

    // Apply color
    painter.setCompositionMode(QPainter::CompositionMode_SourceIn);
    painter.fillRect(pix.rect(), color);
  }

  return pix;
}

// New dynamic icon functions
QIcon ThemeManager::createDynamicIcon(const QString& svgPath,
                                      const QSize& size) {
  QIcon icon;

  // Normal state
  icon.addPixmap(coloredPixmap(svgPath, m_iconColor, size), QIcon::Normal,
                 QIcon::Off);

  // Hover state (Active)
  icon.addPixmap(coloredPixmap(svgPath, m_hoverColor, size), QIcon::Active,
                 QIcon::Off);

  // Pressed state (Selected)
  icon.addPixmap(coloredPixmap(svgPath, m_pressedColor, size), QIcon::Selected,
                 QIcon::Off);

  // Disabled state
  icon.addPixmap(coloredPixmap(svgPath, m_disabledColor, size), QIcon::Disabled,
                 QIcon::Off);

  return icon;
}

void ThemeManager::setupDynamicAction(QAction* action, const QString& svgPath,
                                      const QSize& size) {
  if (!action) return;

  // Register the icon for updates
  IconData iconData;
  iconData.svgPath = svgPath;
  iconData.size = size;
  iconData.widget = action;
  m_registeredIcons[action] = iconData;

  // Set initial icon
  action->setIcon(createDynamicIcon(svgPath, size));

  thinfo() << ("Dynamic icon setup for QAction: " + svgPath);
}

void ThemeManager::setupDynamicButton(QPushButton* button,
                                      const QString& svgPath,
                                      const QSize& size) {
  if (!button) return;

  // Register the icon for updates
  IconData iconData;
  iconData.svgPath = svgPath;
  iconData.size = size;
  iconData.widget = button;
  m_registeredIcons[button] = iconData;

  // Set initial icon
  button->setIcon(createDynamicIcon(svgPath, size));
  button->setIconSize(size);

  thinfo() << ("Dynamic icon setup for QPushButton: " + svgPath);
}

void ThemeManager::setupDynamicButton(QToolButton* button,
                                      const QString& svgPath,
                                      const QSize& size) {
  if (!button) return;

  // Register the icon for updates
  IconData iconData;
  iconData.svgPath = svgPath;
  iconData.size = size;
  iconData.widget = button;
  m_registeredIcons[button] = iconData;

  // Set initial icon
  button->setIcon(createDynamicIcon(svgPath, size));
  button->setIconSize(size);

  thinfo() << ("Dynamic icon setup for QToolButton: " + svgPath);
}

void ThemeManager::setIconColors(const QColor& normal, const QColor& hover,
                                 const QColor& pressed,
                                 const QColor& disabled) {
  m_iconColor = normal;
  m_hoverColor = hover;
  m_pressedColor = pressed;
  m_disabledColor = disabled;

  // Mevcut iconları güncelle
  updateIconColors();

  thinfo() << ("Icon colors updated programmatically");
}

void ThemeManager::parseThemeColors(const QString& qssContent) {
  // Parse QSS content to extract color values
  // Look for custom properties or specific selectors

  static QRegularExpression iconColorRegex(
      R"(\/\*\s*@icon-color:\s*([^*]+)\s*\*\/)");
  static QRegularExpression hoverColorRegex(
      R"(\/\*\s*@hover-color:\s*([^*]+)\s*\*\/)");
  static QRegularExpression pressedColorRegex(
      R"(\/\*\s*@pressed-color:\s*([^*]+)\s*\*\/)");
  static QRegularExpression disabledColorRegex(
      R"(\/\*\s*@disabled-color:\s*([^*]+)\s*\*\/)");

  // Renkleri parse et ve set et
  auto iconMatch = iconColorRegex.match(qssContent);
  if (iconMatch.hasMatch()) {
    QString colorStr = iconMatch.captured(1).trimmed();
    QColor color(colorStr);
    if (color.isValid()) {
      m_iconColor = color;
    }
  }

  auto hoverMatch = hoverColorRegex.match(qssContent);
  if (hoverMatch.hasMatch()) {
    m_hoverColor = QColor(hoverMatch.captured(1).trimmed());
  }

  auto pressedMatch = pressedColorRegex.match(qssContent);
  if (pressedMatch.hasMatch()) {
    m_pressedColor = QColor(pressedMatch.captured(1).trimmed());
  }

  auto disabledMatch = disabledColorRegex.match(qssContent);
  if (disabledMatch.hasMatch()) {
    m_disabledColor = QColor(disabledMatch.captured(1).trimmed());
  }

  thinfo() << ("Theme colors parsed - Icon: " + m_iconColor.name() +
               ", Hover: " + m_hoverColor.name() + ", Pressed: " +
               m_pressedColor.name() + ", Disabled: " + m_disabledColor.name());
}

bool ThemeManager::applyTheme(const QString& visibleName) {
  if (!themeMap.contains(visibleName)) {
    therr() << "Theme not found: " + visibleName;
    return false;
  }

  QString qssFileName = themeMap[visibleName];
  QString filePath = themesDirPath() + "/" + qssFileName + ".qss";
  if (!QFile::exists(filePath)) {
    fserr() << ("Theme file cannot be found: " + filePath);
    return false;
  }

  QString qss = readQssFile(filePath);
  if (qss.isEmpty()) {
    therr() << "QSS file is empty.";
    return false;
  }

  // Parse theme colors before applying
  parseThemeColors(qss);

  qApp->setStyleSheet(qss);
  thinfo() << ("Theme applied: " +
               visibleName);  // PROGRAM EN SON BURDA LOG VERDİ

  // Update all registered icons
  updateIconColors();
  emit themeChanged();

  return true;
}

void ThemeManager::updateIconColors() {
  for (auto it = m_registeredIcons.begin(); it != m_registeredIcons.end();) {
    QPointer<QObject> widget = it.value().widget;

    // Eğer widget artık yoksa kayıtları sil
    if (!widget) {
      it = m_registeredIcons.erase(it);
      continue;
    }

    // Eğer widget hâlâ geçerli ise icon güncelle
    QIcon newIcon = createDynamicIcon(it.value().svgPath, it.value().size);

    if (QAction* action = qobject_cast<QAction*>(widget)) {
      action->setIcon(newIcon);
    } else if (QPushButton* button = qobject_cast<QPushButton*>(widget)) {
      button->setIcon(newIcon);
    } else if (QToolButton* toolButton = qobject_cast<QToolButton*>(widget)) {
      toolButton->setIcon(newIcon);
    }

    ++it;
  }

  thinfo() << "Updated " << m_registeredIcons.size() << " dynamic icons safely";
}

void ThemeManager::refreshAllIcons() { updateIconColors(); }

// Color getters
QColor ThemeManager::getCurrentIconColor() const { return m_iconColor; }

QColor ThemeManager::getCurrentHoverColor() const { return m_hoverColor; }

QColor ThemeManager::getCurrentPressedColor() const { return m_pressedColor; }

QColor ThemeManager::getCurrentDisabledColor() const { return m_disabledColor; }

QMap<QString, QString> ThemeManager::reverseThemeMap() const {
  QMap<QString, QString> reversed;
  for (auto it = themeMap.constBegin(); it != themeMap.constEnd(); ++it) {
    reversed[it.value()] = it.key();
  }
  return reversed;
}

QStringList ThemeManager::availableThemes() const { return themeMap.keys(); }

QString ThemeManager::getFileName(const QString& visibleName) {
  return themeMap[visibleName];
}

QString ThemeManager::getVisibleName(const QString& filename) {
  return reversedThemeMap[filename];
}
