#include "mainwindow.h"

#include <windows.h>

#include <QJsonObject>
#include <QMessageBox>
#include <QPainter>
#include <QResizeEvent>
#include <QTimer>
#include <QtSvg/QSvgRenderer>
#include <QOverload>

#include "LoggerStream.h"
#include "appdatamanager.h"
#include "hotkeyservice.h"
#include "languagemanager.h"
#include "logger.h"
#include "macromanager.h"
#include "macroselectionwin.h"
#include "settingsmanager.h"
#include "settingswin.h"
#include "thememanager.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(const QJsonObject& settings,
                       const QVector<Macro>& macros, QWidget* parent)
    : QMainWindow(parent),
      m_settings(settings),
      m_macros(macros),
      ui(new Ui::MainWindow) {
  ui->setupUi(this);

  // clicker ve hotkey ayarlamaları
  clickEngine = ClickEngine::getInstance();
  hotkeyService = HotkeyService::getInstance();

  // aktif makroyu ayarla
  auto macroQuery =
      MacroManager::instance().getMacroById(getSetting("ActiveMacro").toInt(1));
  if (macroQuery.has_value()) {
    activeMacro = macroQuery.value();
  } else {
    // Fallback: İlk makroyu al veya default bir makro oluştur
    auto allMacros = MacroManager::instance().getAllMacros();
    if (!allMacros.isEmpty()) {
      activeMacro = allMacros.first();
    } else {
      merr() << "No macros found!";
      return;  // veya uygun bir hata işlemi
    }
  }

  // hotkey registeration
  bool isDefKey = activeMacro.hotkey == "DEF";
  QString htkstr =
      isDefKey ? getSetting("DefaultHotkey").toString() : activeMacro.hotkey;
  if (!hotkeyService->registerHotkey(htkstr, activeMacro.id)) {
    hserr() << "Hotkey registeration failed!";
    return;
  }

  // clicker engine start/stop eventleri
  connect(clickEngine, &ClickEngine::macroStarted, this,
          &MainWindow::onMacroStarted);
  connect(clickEngine, &ClickEngine::macroStopped, this,
          &MainWindow::onMacroStopped);

  // hotkey pressed eventleri
  connect(hotkeyService, &HotkeyService::hotkeyPressed, this, [this](int id) {
    hsinfo() << "Hotkey pressed!";
    if (clickEngine->isMacroRunning() &&
        clickEngine->getCurrentMacroId() == id) {
      clickEngine->stopCurrentMacro();
    } else {
      clickEngine->startMacro(id);
    }
  });

  connect(hotkeyService, &HotkeyService::hotkeyRegistrationFailed, this,
          [](int id, const QString& reason) {
            hserr() << "Hotkey registration failed, reason: " << reason
                    << ", id: " << id;
          });

  connect(&LanguageManager::instance(), &LanguageManager::languageChanged, this,
          &MainWindow::retranslateUi);

  // Connect to theme changes
  connect(&ThemeManager::instance(), &ThemeManager::themeChanged, this,
          &MainWindow::onThemeChanged);

  // tablo ui ayarlamaları
  ui->actionsTable->verticalHeader()->setVisible(false);

  // makro ayarlamaları
  setActiveMacro(getSetting("ActiveMacro").toInt(1));

  // UI hazır olana kadar bekle
  QTimer::singleShot(10, this, [this]() {
    if (!m_isClosing) {
      loadLanguage();
      setupDynamicIcons();
      adjustTableColumns();
    }
  });

  ui->actionsTable->setEditTriggers(QAbstractItemView::DoubleClicked |
                                    QAbstractItemView::EditKeyPressed);

  // action table change event listeners
  connect(ui->actionsTable, &QTableWidget::itemChanged, this,
          [this](QTableWidgetItem* item) {
            if (item) {
              int row = item->row();
              int col = item->column();

              qDebug() << "Item changed at row:" << row << "col:" << col; // Debug için

              // Tüm sütunlar için değişikliği kabul et
              m_modifiedRows.insert(row);
              if (!m_hasUnsavedChanges) {
                m_hasUnsavedChanges = true;
                MarkAsUnsaved();
                qDebug() << "MarkAsUnsaved called from itemChanged"; // Debug için
              }
            }
          });

  connect(ui->actionsTable, &QTableWidget::cellChanged, this,
          [this](int row, int column) {
            qDebug() << "Cell changed at row:" << row << "column:" << column; // Debug için
            markRowAsModified(row);
          });


  ui->labelErrors->setVisible(false);
}

void MainWindow::onMacroStarted(int id) { minfo() << "Macro started: " << id; }

void MainWindow::onMacroStopped(int id) { minfo() << "Macro stopped: " << id; }

void MainWindow::onMacroError(int id, const QString& error) {
  merr() << "Macro error in " << id << ": " << error;
}

QString MainWindow::getHotkeyString(QString htkstr) {
  return htkstr == "DEF" ? m_settings["DefaultHotkey"].toString() : htkstr;
}

void MainWindow::setActiveMacro(int id) {
  m_hasUnsavedChanges = false;
  m_modifiedRows.clear();
  m_actionsModified = false;
  m_pendingActions.clear();

  auto macro = MacroManager::instance().getMacroById(id);
  if (macro.has_value()) {
    QString activeMacroName = macro->name;
    ui->actionActiveMacro->setText(
        tr("active macro label").replace("#MCR", activeMacroName) + " (" +
        getHotkeyString(macro->hotkey) + ")");
    activeMacro = macro.value();
    sinfo() << "(MainWindow) Active macro set to: " << activeMacroName
            << " (ID: " << id << ")";
  } else {
    serr() << "(MainWindow) Active macro not found with ID: " << id;
    // Fallback to first macro or default
    if (!m_macros.isEmpty()) {
      auto fallbackMacro = MacroManager::instance().getMacroById(1);
      if (fallbackMacro.has_value()) {
        activeMacro = fallbackMacro.value();
        id = activeMacro.id;
        ui->actionActiveMacro->setText(
            tr("active macro label").replace("#MCR", activeMacro.name) + " (" +
            getHotkeyString(activeMacro.hotkey) + ")");
      }
    }
  }

  ui->actionsTable->setRowCount(0);
  QVector<MacroAction> acts = MacroManager::instance().getActions(id);
  for (const MacroAction& act : acts) {
    addActionToTable(act);
  }

  // reset saved states
  m_hasUnsavedChanges = false;
  m_modifiedRows.clear();
  m_actionsModified = false;
  m_pendingActions.clear();
}

void MainWindow::setupDynamicIcons() {
  // Use assets/icons from the project directory
  QString iconsPath = ":/assets/icons";  // Resource path

  // Setup QActions with dynamic icons
  // active macro
  if (ui->actionActiveMacro) {
    ThemeManager::instance().setupDynamicAction(
        ui->actionActiveMacro, iconsPath + "/select.svg", QSize(24, 24));
  }
  // settings
  if (ui->actionSettings) {
    ThemeManager::instance().setupDynamicAction(
        ui->actionSettings, iconsPath + "/settings.svg", QSize(24, 24));
  }
  // save
  if (ui->actionSave) {
    ThemeManager::instance().setupDynamicAction(
        ui->actionSave, iconsPath + "/save.svg", QSize(24, 24));
  }
  // about
  if (ui->actionAbout) {
    ThemeManager::instance().setupDynamicAction(
        ui->actionAbout, iconsPath + "/info.svg", QSize(24, 24));
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
void MainWindow::addActionToTable(MacroAction a) {
  int row = ui->actionsTable->rowCount();
  ui->actionsTable->insertRow(row);

  QTableWidgetItem* noItem = new QTableWidgetItem(QString::number(a.order));
  noItem->setFlags(noItem->flags() & ~Qt::ItemIsEditable);
  noItem->setTextAlignment(Qt::AlignCenter);
  ui->actionsTable->setItem(row, 0, noItem);

          // Action Type ComboBox
  QWidget* actionTypeContainer = new QWidget();
  QHBoxLayout* actionTypeLayout = new QHBoxLayout(actionTypeContainer);
  actionTypeLayout->setContentsMargins(2, 2, 2, 2);
  actionTypeLayout->setSpacing(0);

  QComboBox* actionTypeCombo = createActionTypeComboBox(a.action_type);
  actionTypeCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  actionTypeLayout->addWidget(actionTypeCombo);

  ui->actionsTable->setCellWidget(row, 1, actionTypeContainer);

          // Click Type ComboBox
  QWidget* clickTypeContainer = new QWidget();
  QHBoxLayout* clickTypeLayout = new QHBoxLayout(clickTypeContainer);
  clickTypeLayout->setContentsMargins(2, 2, 2, 2);
  clickTypeLayout->setSpacing(0);

  QComboBox* clickTypeCombo = createClickTypeComboBox(a.click_type);
  clickTypeCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  clickTypeLayout->addWidget(clickTypeCombo);

  ui->actionsTable->setCellWidget(row, 2, clickTypeContainer);

          // Mouse Button ComboBox
  QWidget* mouseButtonContainer = new QWidget();
  QHBoxLayout* mouseButtonLayout = new QHBoxLayout(mouseButtonContainer);
  mouseButtonLayout->setContentsMargins(2, 2, 2, 2);
  mouseButtonLayout->setSpacing(0);

  std::optional<MouseButton> mouseButtonValue =
      (a.action_type == ActionType::KEYBOARD) ? std::nullopt : a.mouse_button;

  QComboBox* mouseButtonCombo = createMouseButtonComboBox(mouseButtonValue);
  mouseButtonCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  mouseButtonLayout->addWidget(mouseButtonCombo);

  ui->actionsTable->setCellWidget(row, 3, mouseButtonContainer);

          // applyActionTypeConstraints'i en son çağır
  applyActionTypeConstraints(row, a.action_type);

  QTableWidgetItem* repeatItem = new QTableWidgetItem(QString::number(a.repeat));
  repeatItem->setTextAlignment(Qt::AlignCenter);
  ui->actionsTable->setItem(row, 4, repeatItem);

  QTableWidgetItem* intervalItem = new QTableWidgetItem(QString::number(a.interval));
  intervalItem->setTextAlignment(Qt::AlignCenter);
  ui->actionsTable->setItem(row, 5, intervalItem);

          // Additional Settings button
  QWidget* settingsContainer = new QWidget();
  QHBoxLayout* settingsLayout = new QHBoxLayout(settingsContainer);
  settingsLayout->setContentsMargins(1, 1, 1, 1);
  settingsLayout->setSpacing(0);

  QPushButton* settingsBtn = new QPushButton("...");
  settingsBtn->setFixedSize(28, 28);
  settingsBtn->setStyleSheet("font-weight: bold; margin: 0px; padding: 0px;");
  settingsBtn->setToolTip(tr("additionals tooltip"));

  connect(settingsBtn, &QPushButton::clicked, this,
          [this, row]() { showAdditionalSettings(row); });

  settingsLayout->addWidget(settingsBtn, 0, Qt::AlignCenter);
  settingsLayout->setAlignment(Qt::AlignCenter);
  ui->actionsTable->setCellWidget(row, 6, settingsContainer);

  ui->actionsTable->setRowHeight(row, 32);
}
void MainWindow::updateActionFromTableRow(int row) {
  if (row < 0 || row >= ui->actionsTable->rowCount()) return;

  // Mark row as modified and trigger unsaved state
  m_modifiedRows.insert(row);
  if (!m_hasUnsavedChanges) {
    m_hasUnsavedChanges = true;
    MarkAsUnsaved();
  }
}
QComboBox* MainWindow::createActionTypeComboBox(ActionType currentValue) {
  QComboBox* comboBox = new QComboBox();

  comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  comboBox->setMinimumHeight(24);
  comboBox->setMaximumHeight(28);

  comboBox->addItem(tr("atype keyboard"), QVariant::fromValue(ActionType::KEYBOARD));
  comboBox->addItem(tr("atype mouse"), QVariant::fromValue(ActionType::MOUSE));

  int index = comboBox->findData(QVariant::fromValue(currentValue));
  if (index != -1) {
    comboBox->setCurrentIndex(index);
  }

  connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
          [this, comboBox](int index) {
            Q_UNUSED(index)
            qDebug() << "Action type combo changed"; // Debug için

            int targetRow = -1;
            for (int row = 0; row < ui->actionsTable->rowCount(); ++row) {
              QWidget* container = ui->actionsTable->cellWidget(row, 1);
              if (container && container->findChild<QComboBox*>() == comboBox) {
                targetRow = row;
                break;
              }
            }

            if (targetRow >= 0) {
              ActionType selectedType = comboBox->currentData().value<ActionType>();
              applyActionTypeConstraints(targetRow, selectedType);
              markRowAsModified(targetRow);
              qDebug() << "Row " << targetRow << " marked as modified"; // Debug için
            }
          });

  return comboBox;
}
void MainWindow::applyActionTypeConstraints(int row, ActionType actionType) {
  QWidget* clickTypeContainer = ui->actionsTable->cellWidget(row, 2);
  QComboBox* clickTypeCombo = nullptr;
  if (clickTypeContainer) {
    clickTypeCombo = clickTypeContainer->findChild<QComboBox*>();
  }

  QWidget* mouseButtonContainer = ui->actionsTable->cellWidget(row, 3);
  QComboBox* mouseButtonCombo = nullptr;
  if (mouseButtonContainer) {
    mouseButtonCombo = mouseButtonContainer->findChild<QComboBox*>();
  }

  if (actionType == ActionType::KEYBOARD) {
    if (mouseButtonCombo) {
      mouseButtonCombo->setEnabled(false);
      mouseButtonCombo->setCurrentIndex(0);
      mouseButtonCombo->clear();
      mouseButtonCombo->addItem(tr("msbtn -"), QVariant());
    }

    if (clickTypeCombo) {
      ClickType currentClickType = clickTypeCombo->currentData().value<ClickType>();

      clickTypeCombo->blockSignals(true);
      clickTypeCombo->clear();

      clickTypeCombo->addItem(tr("ctype click"), QVariant::fromValue(ClickType::CLICK));
      clickTypeCombo->addItem(tr("ctype hold"), QVariant::fromValue(ClickType::HOLD));

      if (currentClickType == ClickType::HOVER) {
        clickTypeCombo->setCurrentIndex(0);
      } else {
        int index = clickTypeCombo->findData(QVariant::fromValue(currentClickType));
        clickTypeCombo->setCurrentIndex(index != -1 ? index : 0);
      }

      clickTypeCombo->blockSignals(false);
    }

  } else if (actionType == ActionType::MOUSE) {
    if (mouseButtonCombo) {
      mouseButtonCombo->setEnabled(true);
      mouseButtonCombo->setStyleSheet("");

      std::optional<MouseButton> currentValue;
      if (mouseButtonCombo->currentData().isValid()) {
        currentValue = mouseButtonCombo->currentData().value<MouseButton>();
      }

      mouseButtonCombo->blockSignals(true);
      mouseButtonCombo->clear();

      mouseButtonCombo->addItem(tr("msbtn left"), QVariant::fromValue(MouseButton::LEFT));
      mouseButtonCombo->addItem(tr("msbtn right"), QVariant::fromValue(MouseButton::RIGHT));
      mouseButtonCombo->addItem(tr("msbtn mid"), QVariant::fromValue(MouseButton::MID));

      if (currentValue.has_value()) {
        int index = mouseButtonCombo->findData(QVariant::fromValue(currentValue.value()));
        mouseButtonCombo->setCurrentIndex(index != -1 ? index : 0);
      } else {
        mouseButtonCombo->setCurrentIndex(0);
      }

      mouseButtonCombo->blockSignals(false);
    }

    if (clickTypeCombo) {
      ClickType currentClickType = clickTypeCombo->currentData().value<ClickType>();

      clickTypeCombo->blockSignals(true);
      clickTypeCombo->clear();

      clickTypeCombo->addItem(tr("ctype click"), QVariant::fromValue(ClickType::CLICK));
      clickTypeCombo->addItem(tr("ctype hover"), QVariant::fromValue(ClickType::HOVER));
      clickTypeCombo->addItem(tr("ctype hold"), QVariant::fromValue(ClickType::HOLD));

      int index = clickTypeCombo->findData(QVariant::fromValue(currentClickType));
      clickTypeCombo->setCurrentIndex(index != -1 ? index : 0);

      clickTypeCombo->blockSignals(false);
    }
  }
}
QComboBox* MainWindow::createClickTypeComboBox(ClickType currentValue) {
  QComboBox* comboBox = new QComboBox();

  comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  comboBox->setMinimumHeight(24);
  comboBox->setMaximumHeight(28);

  comboBox->addItem(tr("ctype click"), QVariant::fromValue(ClickType::CLICK));
  comboBox->addItem(tr("ctype hover"), QVariant::fromValue(ClickType::HOVER));
  comboBox->addItem(tr("ctype hold"), QVariant::fromValue(ClickType::HOLD));

  int index = comboBox->findData(QVariant::fromValue(currentValue));
  if (index != -1) {
    comboBox->setCurrentIndex(index);
  }

  connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          [this, comboBox](int index) {
            Q_UNUSED(index)
            qDebug() << "Click type combo changed"; // Debug için

            for (int row = 0; row < ui->actionsTable->rowCount(); ++row) {
              QWidget* container = ui->actionsTable->cellWidget(row, 2);
              if (container && container->findChild<QComboBox*>() == comboBox) {
                markRowAsModified(row);
                break;
              }
            }
          }, Qt::QueuedConnection); // QueuedConnection ekleyin


  return comboBox;
}
QComboBox* MainWindow::createMouseButtonComboBox(std::optional<MouseButton> currentValue) {
  QComboBox* comboBox = new QComboBox();

  comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  comboBox->setMinimumHeight(24);
  comboBox->setMaximumHeight(28);

  comboBox->addItem(tr("msbtn -"), QVariant());
  comboBox->addItem(tr("msbtn left"), QVariant::fromValue(MouseButton::LEFT));
  comboBox->addItem(tr("msbtn right"), QVariant::fromValue(MouseButton::RIGHT));
  comboBox->addItem(tr("msbtn mid"), QVariant::fromValue(MouseButton::MID));

  if (currentValue.has_value()) {
    int index = comboBox->findData(QVariant::fromValue(currentValue.value()));
    if (index != -1) {
      comboBox->setCurrentIndex(index);
    }
  } else {
    comboBox->setCurrentIndex(0);
  }

          // DÜZELTME: Qt::QueuedConnection kullanın
  connect(comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          [this, comboBox](int index) {
            Q_UNUSED(index)
            qDebug() << "Mouse button combo changed"; // Debug için

            for (int row = 0; row < ui->actionsTable->rowCount(); ++row) {
              QWidget* container = ui->actionsTable->cellWidget(row, 3);
              if (container && container->findChild<QComboBox*>() == comboBox) {
                markRowAsModified(row);
                break;
              }
            }
          }, Qt::QueuedConnection); // QueuedConnection ekleyin

  return comboBox;
}

MacroAction MainWindow::getActionFromRow(int row) {
  MacroAction a;

  QTableWidgetItem* noItem = ui->actionsTable->item(row, 0);
  a.order = noItem ? noItem->text().toInt() : 0;

          // Action Type
  QWidget* actionTypeContainer = ui->actionsTable->cellWidget(row, 1);
  if (actionTypeContainer) {
    QComboBox* actionCombo = actionTypeContainer->findChild<QComboBox*>();
    if (actionCombo) {
      a.action_type = actionCombo->currentData().value<ActionType>();
      minfo() << "Row" << row << "action_type:" << actionTypeToStr(a.action_type);
    }
  }

          // Click Type
  QWidget* clickTypeContainer = ui->actionsTable->cellWidget(row, 2);
  if (clickTypeContainer) {
    QComboBox* clickCombo = clickTypeContainer->findChild<QComboBox*>();
    if (clickCombo) {
      a.click_type = clickCombo->currentData().value<ClickType>();
      minfo() << "Row" << row << "click_type:" << clickTypeToStr(a.click_type);
    }
  }

          // Mouse Button
  QWidget* mouseButtonContainer = ui->actionsTable->cellWidget(row, 3);
  if (mouseButtonContainer) {
    QComboBox* mouseButtonCombo = mouseButtonContainer->findChild<QComboBox*>();
    if (mouseButtonCombo) {
      if (a.action_type == ActionType::KEYBOARD) {
        a.mouse_button = std::nullopt;
      } else {
        if (mouseButtonCombo->currentData().isValid()) {
          a.mouse_button = mouseButtonCombo->currentData().value<MouseButton>();
          minfo() << "Row" << row << "mouse_button:" << mouseButtonToStr(a.mouse_button.value());
        } else {
          a.mouse_button = std::nullopt;
          minfo() << "Row" << row << "mouse_button: NULL";
        }
      }
    }
  }

  QTableWidgetItem* repeatItem = ui->actionsTable->item(row, 4);
  a.repeat = repeatItem ? repeatItem->text().toInt() : 1;

  QTableWidgetItem* intervalItem = ui->actionsTable->item(row, 5);
  a.interval = intervalItem ? intervalItem->text().toInt() : 0;

          // Diğer alanlar için default değerler
  a.position = "0, 0";
  a.current_pos = true;
  a.hold_duration = 1000;
  a.hover_duration = 1000;
  a.key_name = "A";
  a.macro_id = activeMacro.id;

  return a;
}

QJsonValue MainWindow::getSetting(const QString& key) const {
  return m_settings.value(key);
}

void MainWindow::setSetting(const QString& key, const QJsonValue& value) {
  m_settings[key] = value;
}

void MainWindow::saveActions(int macroId, const QVector<MacroAction>& actions) {
  QString err;
  if (!MacroManager::instance().setActionsForMacro(macroId, actions, &err)) {
    merr() << err;
    QMessageBox::critical(this, tr("error"), err);
  }
}

void MainWindow::markRowAsModified(int row) {
  qDebug() << "markRowAsModified called for row:" << row; // Debug için

  m_modifiedRows.insert(row);
  if (!m_hasUnsavedChanges) {
    m_hasUnsavedChanges = true;
    MarkAsUnsaved();
    qDebug() << "MarkAsUnsaved called from markRowAsModified"; // Debug için
  }
}

void MainWindow::showAdditionalSettings(int row) {
  // pending yoksa bir kere yükle
  if (!m_actionsModified) {
    m_pendingActions = MacroManager::instance().getActions(activeMacro.id);
    m_actionsModified = true;
  }
  if (row < 0 || row >= m_pendingActions.size()) return;

  const MacroAction original = m_pendingActions[row];

  AdditionalSettingsDialog dialog(original, this);
  if (dialog.exec() == QDialog::Accepted) {
    MacroAction updated = dialog.getUpdatedAction();

    if (updated != original) { // eşitlik operatörünü kullandık
      m_pendingActions[row] = updated;
      markRowAsModified(row);
      // burada tabloyu komple refresh etmiyoruz; gerekirse sadece görsel ipucu ver
      qDebug() << "Additional settings changed for row:" << row;
    } else {
      qDebug() << "No changes in additional settings";
    }
  }
}


void MainWindow::loadLanguage() {
  // TOOLBAR ACTION ÇEVİRİLERİ
  ui->actionAbout->setText(tr("about"));
  ui->actionSettings->setText(tr("settings"));
  ui->actionSave->setText(tr("macro save"));

  // tooltipler
  ui->actionsTable->horizontalHeaderItem(0)->setToolTip("No");
  ui->actionsTable->horizontalHeaderItem(1)->setToolTip(
      tr("action type tooltip"));
  ui->actionsTable->horizontalHeaderItem(2)->setToolTip(
      tr("click type tooltip"));
  ui->actionsTable->horizontalHeaderItem(3)->setToolTip(
      tr("mouse button tooltip"));
  ui->actionsTable->horizontalHeaderItem(4)->setToolTip(tr("repeat tooltip"));
  ui->actionsTable->horizontalHeaderItem(5)->setToolTip(tr("interval tooltip"));
  ui->actionsTable->horizontalHeaderItem(6)->setToolTip(
      tr("additionals tooltip"));

  ui->actionActiveMacro->setToolTip(tr("active macro label tooltip"));
  ui->actionSave->setToolTip(tr("macro save tooltip"));
  ui->actionSettings->setToolTip(tr("settings tooltip"));
  ui->actionAbout->setToolTip(tr("about tooltip"));

  ui->btnAddAction->setToolTip(tr("add action tooltip"));
  ui->btnDeleteAction->setToolTip(tr("delete action tooltip"));
  ui->btnEditAction->setToolTip(tr("edit orders tooltip"));

  // headerlar
  ui->actionsTable->setHorizontalHeaderLabels(
      QStringList() << "No" << tr("action type") << tr("click type")
                    << tr("mouse button") << tr("repeat") << tr("interval")
                    << tr("additionals"));

  // BUTONLAR
  ui->btnAddAction->setText(tr("add action"));
  ui->btnDeleteAction->setText(tr("delete action"));
  ui->btnEditAction->setText(tr("edit action orders"));
}

void MainWindow::retranslateUi() {
  ui->retranslateUi(this);
  loadLanguage();
}

void MainWindow::adjustTableColumns() {
  int tableWidth = ui->actionsTable->viewport()->width();

  float noPerc = 0.05f;
  float actionTypePerc = 0.20f;
  float clickTypePerc = 0.20f;
  float msbtnPerc = 0.20f;
  float repeatPerc = 0.13f;
  float intervalPerc = 0.13f;
  float additionalPerc = 1.0f - (noPerc + actionTypePerc + clickTypePerc +
                                 repeatPerc + msbtnPerc + intervalPerc);

  ui->actionsTable->setColumnWidth(0, tableWidth * noPerc);  // Order
  ui->actionsTable->setColumnWidth(1,
                                   tableWidth * actionTypePerc);  // action type
  ui->actionsTable->setColumnWidth(2,
                                   tableWidth * clickTypePerc);  // click type
  ui->actionsTable->setColumnWidth(3, tableWidth * msbtnPerc);   // Mouse button
  ui->actionsTable->setColumnWidth(4, tableWidth * repeatPerc);  // repeat
  ui->actionsTable->setColumnWidth(5, tableWidth * intervalPerc);  // interval
  ui->actionsTable->setColumnWidth(6,
                                   tableWidth * additionalPerc);  // additional
}

void MainWindow::resizeEvent(QResizeEvent* event) {
  QMainWindow::resizeEvent(event);
  adjustTableColumns();
}

MainWindow::~MainWindow() {
  QString settingsPath = AppDataManager::instance().settingsFilePath();
  if (SettingsManager::instance().saveSettings(settingsPath, m_settings)) {
    sinfo() << "The settings file has been synchronized with the UI.";
  } else {
    serr() << "Failed to synchronize the settings file with the UI.”";
  }

  info() << "Closing the app...";
  delete ui;
}

void MainWindow::on_actionSettings_triggered() {
  SettingsWin settingsDialog(m_settings, this);
  int result = settingsDialog.exec();

  if (result == QDialog::Accepted) {
    // Settings dialog'dan güncellenmiş ayarları al
    QJsonObject updatedSettings = settingsDialog.m_settings;

    // Ayarları güncelle
    m_settings = updatedSettings;

    SettingsManager::instance().saveSettings(
        AppDataManager::instance().settingsFilePath(), updatedSettings);

    // UI güncellemeleri
    // aktif makro
    std::optional<Macro> query = MacroManager::instance().getMacroById(
        m_settings["ActiveMacro"].toInt(1));
    Macro activeMacro;
    if (query.has_value()) {
      activeMacro = query.value();
    }
    setActiveMacro(activeMacro.id);

    // hotkey
    bool isDefaultHotkey = activeMacro.hotkey == "DEF";
    QString htkstr = isDefaultHotkey ? m_settings["DefaultHotkey"].toString()
                                     : activeMacro.hotkey;

    if (!hotkeyService->changeHotkey(activeMacro.id, htkstr)) {
      hserr() << "Hotkey change failed after settings tab!";
      return;
    }
  }
}

void MainWindow::refreshMacros() {
  m_macros = MacroManager::instance().getAllMacros();
  Logger::instance().mInfo(
      QString("Macros refreshed. Count: %1").arg(m_macros.size()));
}

void MainWindow::on_actionActiveMacro_triggered() {
  refreshMacros();

  MacroSelectionWin selectionDialog(m_macros,
                                    getSetting("ActiveMacro").toInt(1), this);
  int res = selectionDialog.exec();

  if (res == QDialog::Accepted) {
    // selectionDialog.activeMacroId gerçek SQL ID'si
    int newActiveMacroId = selectionDialog.activeMacroId;
    int oldMacroId = activeMacro.id;

    // Aktif makroyu ayarla
    setActiveMacro(newActiveMacroId);
    setSetting("ActiveMacro", newActiveMacroId);

    QString htkStr = activeMacro.hotkey == "DEF"
                         ? getSetting("DefaultHotkey").toString()
                         : activeMacro.hotkey;

    // eski makronun hotkeyini unregister et
    if (!hotkeyService->unregisterHotkey(oldMacroId)) {
      Logger::instance().hsError(
          "Old macro's hotkey unregisteration failed after macro changing!");
      return;
    }
    // yeni makronun hotkeyini register et
    if (!hotkeyService->registerHotkey(htkStr, newActiveMacroId)) {
      Logger::instance().hsError(
          "New macro's hotkey registeration failed after macro changing!");
      return;
    }

    Logger::instance().mInfo("Active macro set to ID: " +
                             QString::number(newActiveMacroId));
  }
}

#include "aboutdialog.h"

void MainWindow::on_actionAbout_triggered() {
  AboutDialog dlg(this);
  dlg.exec();
}

void MainWindow::on_btnAddAction_clicked() {
  // Load current actions to memory if first modification
  if (!m_actionsModified) {
    m_pendingActions = MacroManager::instance().getActions(activeMacro.id);
    m_actionsModified = true;
  }

  MacroAction a;
  a.action_type = ActionType::MOUSE;  // Default to MOUSE
  a.click_type = ClickType::CLICK;
  a.click_count = 1;
  a.current_pos = true;
  a.hold_duration = 1000;
  a.hover_duration = 1000;
  a.interval = 100;
  a.repeat = 0;
  a.position = "0, 0";
  a.macro_id = activeMacro.id;
  a.key_name = std::nullopt;

  // Set mouse_button based on action_type
  if (a.action_type == ActionType::KEYBOARD) {
    a.mouse_button = std::nullopt;
  } else {
    a.mouse_button = MouseButton::LEFT;  // Default for MOUSE actions
  }

  a.order = m_pendingActions.size();

  // Add to memory
  m_pendingActions.append(a);

  // Add to UI
  addActionToTable(a);

  // Mark as unsaved
  if (!m_hasUnsavedChanges) {
    m_hasUnsavedChanges = true;
    MarkAsUnsaved();
  }

  ui->labelErrors->setVisible(false);
  minfo() << "Action added to pending list with order " << a.order;
}

void MainWindow::renumberActionsTable() {
  for (int row = 0; row < ui->actionsTable->rowCount(); ++row) {
    QTableWidgetItem* noItem = ui->actionsTable->item(row, 0);
    if (noItem) {
      noItem->setText(QString::number(row));
    }
  }
}

void MainWindow::MarkAsUnsaved() {
  qDebug() << "MarkAsUnsaved called - current hasUnsavedChanges:" << m_hasUnsavedChanges;

  m_hasUnsavedChanges = true;

  QString unsavedText = tr("active macro label").replace("#MCR", activeMacro.name) + "* (" +
                        getHotkeyString(activeMacro.hotkey) + ")";

  qDebug() << "Setting unsaved text:" << unsavedText;
  ui->actionActiveMacro->setText(unsavedText);

  QFont f = ui->actionActiveMacro->font();
  f.setBold(true);
  ui->actionActiveMacro->setFont(f);

  qDebug() << "UI updated to unsaved state";
}

void MainWindow::MarkAsSaved() {
  qDebug() << "MarkAsSaved called";

  m_hasUnsavedChanges = false;
  m_modifiedRows.clear();
  m_actionsModified = false;
  m_pendingActions.clear();

  QString savedText = tr("active macro label").replace("#MCR", activeMacro.name) + " (" +
                      getHotkeyString(activeMacro.hotkey) + ")";

  qDebug() << "Setting saved text:" << savedText;
  ui->actionActiveMacro->setText(savedText);

  QFont f = ui->actionActiveMacro->font();
  f.setBold(false);
  ui->actionActiveMacro->setFont(f);

  qDebug() << "UI updated to saved state";
}

void MainWindow::on_btnDeleteAction_clicked() {
  QList<QTableWidgetItem*> selected = ui->actionsTable->selectedItems();
  if (selected.isEmpty()) {
    ui->labelErrors->setVisible(true);
    ui->labelErrors->setText(tr("select an action to del"));
    MessageBeep(MB_ICONHAND);
    return;
  }

  int row = selected.first()->row();

  // Eğer ilk kez değişiklik yapılıyorsa, mevcut actions'ları memory'e yükle
  if (!m_actionsModified) {
    m_pendingActions = MacroManager::instance().getActions(activeMacro.id);
    m_actionsModified = true;
  }

  if (row >= 0 && row < m_pendingActions.size()) {
    // Memory'den sil
    m_pendingActions.removeAt(row);

    // Order'ları yeniden düzenle
    for (int i = row; i < m_pendingActions.size(); ++i) {
      m_pendingActions[i].order = i;
    }

    // UI'dan sil
    ui->actionsTable->removeRow(row);
    renumberActionsTable();

    // Unsaved olarak işaretle
    if (!m_hasUnsavedChanges) {
      m_hasUnsavedChanges = true;
      MarkAsUnsaved();
    }

    ui->labelErrors->setVisible(false);
    minfo() << "Action removed from pending list at row " << row;
  }
}

void MainWindow::on_actionSave_triggered() {
  if (!m_hasUnsavedChanges) return;

  QString err;
  QVector<MacroAction> base =
      m_actionsModified
          ? m_pendingActions
          : MacroManager::instance().getActions(activeMacro.id);

  QVector<MacroAction> actionsToSave;
  actionsToSave.reserve(ui->actionsTable->rowCount());

  for (int row = 0; row < ui->actionsTable->rowCount(); ++row) {
    MacroAction a = (row >= 0 && row < base.size()) ? base[row] : MacroAction{};

            // görünür sütunları tablodan oku
    QTableWidgetItem* noItem = ui->actionsTable->item(row, 0);
    a.order = noItem ? noItem->text().toInt() : row;

            // action type
    if (QWidget* w = ui->actionsTable->cellWidget(row, 1)) {
      if (auto combo = w->findChild<QComboBox*>())
        a.action_type = combo->currentData().value<ActionType>();
    }

            // click type
    if (QWidget* w = ui->actionsTable->cellWidget(row, 2)) {
      if (auto combo = w->findChild<QComboBox*>())
        a.click_type = combo->currentData().value<ClickType>();
    }

            // mouse button (action_type KEYBOARD ise nullopt)
    if (a.action_type == ActionType::KEYBOARD) {
      a.mouse_button = std::nullopt;
    } else {
      if (QWidget* w = ui->actionsTable->cellWidget(row, 3)) {
        if (auto combo = w->findChild<QComboBox*>()) {
          a.mouse_button = combo->currentData().isValid()
          ? std::optional<MouseButton>(combo->currentData().value<MouseButton>())
          : std::nullopt;
        }
      }
    }

            // repeat / interval
    if (auto it = ui->actionsTable->item(row, 4)) a.repeat = it->text().toInt();
    if (auto it = ui->actionsTable->item(row, 5)) a.interval = it->text().toInt();

            // temel id/order
    a.macro_id = activeMacro.id;
    a.order = row;

    actionsToSave.append(a);
  }

  if (!MacroManager::instance().setActionsForMacro(activeMacro.id, actionsToSave, &err)) {
    merr() << "Failed to save actions: " << err;
    ui->labelErrors->setVisible(true);
    ui->labelErrors->setText(QString("Save failed: %1").arg(err));
    QMessageBox::critical(this, tr("error"),
                          QString("Failed to save actions: %1").arg(err));
    MacroManager::instance().normalizeOrders(activeMacro.id, nullptr);
    return;
  }

          // temizle
  m_hasUnsavedChanges = false;
  m_modifiedRows.clear();
  m_actionsModified = false;
  m_pendingActions.clear();
  MarkAsSaved();
  ui->labelErrors->setVisible(false);
}
