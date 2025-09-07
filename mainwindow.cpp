#include "mainwindow.h"

#include <windows.h>

#include <QJsonObject>
#include <QMessageBox>
#include <QOverload>
#include <QPainter>
#include <QResizeEvent>
#include <QThread>
#include <QTimer>
#include <QtSvg/QSvgRenderer>

#include "Enums.h"
#include "LoggerStream.h"
#include "additionalsettingsdialog.h"
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
#include "instances.h"

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
      _macroman().getMacroById(getSetting("ActiveMacro").toInt(1));
  if (macroQuery.has_value()) {
    activeMacro = macroQuery.value();
  } else {
    // Fallback: İlk makroyu al veya default bir makro oluştur
    auto allMacros = _macroman().getAllMacros();
    if (!allMacros.isEmpty()) {
      activeMacro = allMacros.first();
    } else {
      merr() << "No macros found!";
      return;
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

  // hotkey eventleri
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

  // connect to language changes
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
      MarkAsSaved();

      QTimer::singleShot(100, this, [this]() { validateCurrentActions(); });
    }
  });

  ui->actionsTable->setEditTriggers(QAbstractItemView::DoubleClicked |
                                    QAbstractItemView::EditKeyPressed);

  // action table change event listeners
  connect(ui->actionsTable, &QTableWidget::itemChanged, this,
          [this](QTableWidgetItem* item) {
            if (item) {
              int row = item->row();

              // Tüm sütunlar için değişikliği kabul et
              m_modifiedRows.insert(row);
              if (!m_hasUnsavedChanges) {
                m_hasUnsavedChanges = true;
                MarkAsUnsaved();
              }
            }
          });

  connect(ui->actionsTable, &QTableWidget::cellChanged, this,
          [this](int row) { markRowAsModified(row); });

  ui->labelErrors->setVisible(false);
}

void MainWindow::validateCurrentActions() {
  if (!checkPointlessCycles()) return;

  ui->labelErrors->setVisible(false);
}

void MainWindow::lockOrUnlockUI() {
  bool e = !isMacroRunning;
  ui->actionActiveMacro->setEnabled(e);
  ui->actionSettings->setEnabled(e);
  ui->actionsTable->setEnabled(e);
  ui->btnAddAction->setEnabled(e);
  ui->btnDeleteAction->setEnabled(e);
  ui->btnEditAction->setEnabled(e);
  ui->actionSave->setEnabled(e);
}

void MainWindow::onMacroStarted(int id) {
  isMacroRunning = true;
  minfo() << "Macro started: " << id;
  this->setWindowTitle("Auto Clicker 2.0 - " + tr("Running"));
  lockOrUnlockUI();
}

void MainWindow::onMacroStopped(int id) {
  isMacroRunning = false;
  minfo() << "Macro stopped: " << id;
  this->setWindowTitle("Auto Clicker 2.0");
  lockOrUnlockUI();
}

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

  auto macro = _macroman().getMacroById(id);
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
      auto fallbackMacro = _macroman().getMacroById(1);
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
  QVector<MacroAction> acts = _macroman().getActions(id);
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
  QString iconsPath = ":/assets/icons";  // Resource path

  // Setup QActions with dynamic icons
  // active macro
  if (ui->actionActiveMacro) {
    _themesman().setupDynamicAction(
        ui->actionActiveMacro, iconsPath + "/select.svg", QSize(24, 24));
  }
  // settings
  if (ui->actionSettings) {
    _themesman().setupDynamicAction(
        ui->actionSettings, iconsPath + "/settings.svg", QSize(24, 24));
  }
  // save
  if (ui->actionSave) {
    _themesman().setupDynamicAction(
        ui->actionSave, iconsPath + "/save.svg", QSize(24, 24));
  }
  // about
  if (ui->actionAbout) {
    _themesman().setupDynamicAction(
        ui->actionAbout, iconsPath + "/info.svg", QSize(24, 24));
  }

  thinfo() << "Dynamic icons setup completed";
}

void MainWindow::onThemeChanged() { refreshIcons(); }

void MainWindow::refreshIcons() {
  // Force refresh all icons if needed
  _themesman().refreshAllIcons();
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

  QTableWidgetItem* repeatItem =
      new QTableWidgetItem(QString::number(a.repeat));
  repeatItem->setTextAlignment(Qt::AlignCenter);
  ui->actionsTable->setItem(row, 4, repeatItem);

  QTableWidgetItem* intervalItem =
      new QTableWidgetItem(QString::number(a.interval));
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

  QTimer::singleShot(50, this, [this]() { validateCurrentActions(); });
}
QComboBox* MainWindow::createActionTypeComboBox(ActionType currentValue) {
  QComboBox* comboBox = new QComboBox();

  comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  comboBox->setMinimumHeight(24);
  comboBox->setMaximumHeight(28);

  comboBox->addItem(tr("atype keyboard"),
                    QVariant::fromValue(ActionType::KEYBOARD));
  comboBox->addItem(tr("atype mouse"), QVariant::fromValue(ActionType::MOUSE));

  int index = comboBox->findData(QVariant::fromValue(currentValue));
  if (index != -1) {
    comboBox->setCurrentIndex(index);
  }

  connect(
      comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
      [this, comboBox](int index) {
        Q_UNUSED(index)

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
        }
        QTimer::singleShot(100, this, [this]() { validateCurrentActions(); });
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
      mouseButtonCombo->addItem(tr("msbtn -"), "");
    }

    if (clickTypeCombo) {
      ClickType currentClickType =
          clickTypeCombo->currentData().value<ClickType>();

      clickTypeCombo->blockSignals(true);
      clickTypeCombo->clear();

      clickTypeCombo->addItem(tr("ctype click"),
                              QVariant::fromValue(ClickType::CLICK));
      clickTypeCombo->addItem(tr("ctype hold"),
                              QVariant::fromValue(ClickType::HOLD));

      if (currentClickType == ClickType::HOVER) {
        clickTypeCombo->setCurrentIndex(0);
      } else {
        int index =
            clickTypeCombo->findData(QVariant::fromValue(currentClickType));
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

      mouseButtonCombo->addItem(tr("msbtn left"),
                                QVariant::fromValue(MouseButton::LEFT));
      mouseButtonCombo->addItem(tr("msbtn right"),
                                QVariant::fromValue(MouseButton::RIGHT));
      mouseButtonCombo->addItem(tr("msbtn mid"),
                                QVariant::fromValue(MouseButton::MID));

      if (currentValue.has_value()) {
        int index = mouseButtonCombo->findData(
            QVariant::fromValue(currentValue.value()));
        mouseButtonCombo->setCurrentIndex(index != -1 ? index : 0);
      } else {
        mouseButtonCombo->setCurrentIndex(0);
      }

      mouseButtonCombo->blockSignals(false);
    }

    if (clickTypeCombo) {
      ClickType currentClickType =
          clickTypeCombo->currentData().value<ClickType>();

      clickTypeCombo->blockSignals(true);
      clickTypeCombo->clear();

      clickTypeCombo->addItem(tr("ctype click"),
                              QVariant::fromValue(ClickType::CLICK));
      clickTypeCombo->addItem(tr("ctype hover"),
                              QVariant::fromValue(ClickType::HOVER));
      clickTypeCombo->addItem(tr("ctype hold"),
                              QVariant::fromValue(ClickType::HOLD));

      int index =
          clickTypeCombo->findData(QVariant::fromValue(currentClickType));
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

  connect(
      comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
      [this, comboBox](int index) {
        Q_UNUSED(index)

        for (int row = 0; row < ui->actionsTable->rowCount(); ++row) {
          QWidget* container = ui->actionsTable->cellWidget(row, 2);
          if (container && container->findChild<QComboBox*>() == comboBox) {
            markRowAsModified(row);
            break;
          }
        }
        QTimer::singleShot(100, this, [this]() { validateCurrentActions(); });
      },
      Qt::QueuedConnection);  // QueuedConnection ekleyin

  return comboBox;
}
QComboBox* MainWindow::createMouseButtonComboBox(
    std::optional<MouseButton> currentValue) {
  QComboBox* comboBox = new QComboBox();

  comboBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
  comboBox->setMinimumHeight(24);
  comboBox->setMaximumHeight(28);

  comboBox->addItem(tr("msbtn -"), "");
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

  connect(
      comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this,
      [this, comboBox](int index) {
        Q_UNUSED(index)

        for (int row = 0; row < ui->actionsTable->rowCount(); ++row) {
          QWidget* container = ui->actionsTable->cellWidget(row, 3);
          if (container && container->findChild<QComboBox*>() == comboBox) {
            markRowAsModified(row);
            break;
          }
        }
        QTimer::singleShot(100, this, [this]() { validateCurrentActions(); });
      },
      Qt::QueuedConnection);

  return comboBox;
}

MacroAction MainWindow::getActionFromRow(int row) {
  MacroAction a;

  // Eğer m_pendingActions'da var ise oradan base al, yoksa default
  if (m_actionsModified && row < m_pendingActions.size()) {
    a = m_pendingActions[row];  // Mevcut tüm değerleri al
  } else {
    // Database'den al
    QVector<MacroAction> dbActions =
        _macroman().getActions(activeMacro.id);
    if (row < dbActions.size()) {
      a = dbActions[row];
    } else {
      // Yeni action için default değerler
      a.action_type = ActionType::MOUSE;
      a.click_type = ClickType::CLICK;
      a.click_count = 1;
      a.current_pos = true;
      a.hold_duration = 1000;
      a.interval = 100;
      a.repeat = 1;
      a.position = "0, 0";
      a.macro_id = activeMacro.id;
      a.key_name = "A";
      a.mouse_button = MouseButton::LEFT;
    }
  }

  // UI'dan görünür sütunları oku ve güncelle
  QTableWidgetItem* noItem = ui->actionsTable->item(row, 0);
  a.order = noItem ? noItem->text().toInt() : row;

  // Action Type
  if (QWidget* w = ui->actionsTable->cellWidget(row, 1)) {
    if (auto combo = w->findChild<QComboBox*>())
      a.action_type = combo->currentData().value<ActionType>();
  }

  // Click Type
  if (QWidget* w = ui->actionsTable->cellWidget(row, 2)) {
    if (auto combo = w->findChild<QComboBox*>())
      a.click_type = combo->currentData().value<ClickType>();
  }

  // Mouse Button (action_type KEYBOARD ise nullopt)
  if (a.action_type == ActionType::KEYBOARD) {
    a.mouse_button = std::nullopt;
  } else {
    if (QWidget* w = ui->actionsTable->cellWidget(row, 3)) {
      if (auto combo = w->findChild<QComboBox*>()) {
        a.mouse_button = combo->currentData().isValid()
                             ? std::optional<MouseButton>(
                                   combo->currentData().value<MouseButton>())
                             : std::nullopt;
      }
    }
  }

  // Repeat / Interval
  if (auto it = ui->actionsTable->item(row, 4)) {
    bool ok;
    a.repeat = it->text().toInt(&ok);
    if (!ok) {
      showmsg(tr("Repeat value isn't an integer."), MessageType::ERR);
    }
  }
  if (auto it = ui->actionsTable->item(row, 5)) {
    bool ok;
    a.interval = it->text().toInt(&ok);
    if (!ok) {
      showmsg(tr("Integer value isn't an integer."), MessageType::ERR);
    }
  }

  // Macro ID ve Order
  a.macro_id = activeMacro.id;

  return a;
}

void MainWindow::showmsg(QString str, MessageType t) {
  ui->labelErrors->setProperty("statusType", "");
  ui->labelErrors->setProperty("reorderMode", "");

  ui->labelErrors->setProperty("statusType", msgtypeToStr(t));

  ui->labelErrors->style()->unpolish(ui->labelErrors);
  ui->labelErrors->style()->polish(ui->labelErrors);

  ui->labelErrors->setText(str);
  ui->labelErrors->setVisible(true);
  if (t == MessageType::ERR) MessageBeep(MB_ICONHAND);
}

QJsonValue MainWindow::getSetting(const QString& key) const {
  return m_settings.value(key);
}

void MainWindow::setSetting(const QString& key, const QJsonValue& value) {
  m_settings[key] = value;
}

void MainWindow::saveActions(int macroId, const QVector<MacroAction>& actions) {
  QString err;
  if (!_macroman().setActionsForMacro(macroId, actions, &err)) {
    merr() << err;
    QMessageBox::critical(this, tr("error"), err);
  }
}

void MainWindow::markRowAsModified(int row) {
  // İlk kez değişiklik yapılıyorsa pending actions'ı yükle
  if (!m_actionsModified) {
    m_pendingActions = _macroman().getActions(activeMacro.id);
    m_actionsModified = true;
  }

  // Eğer row pending actions'da varsa, UI'dan güncelle
  if (row < m_pendingActions.size()) {
    MacroAction updated = getActionFromRow(row);
    m_pendingActions[row] = updated;
  }

  m_modifiedRows.insert(row);
  if (!m_hasUnsavedChanges) {
    m_hasUnsavedChanges = true;
    MarkAsUnsaved();
  }
}

void MainWindow::showAdditionalSettings(int row) {
  // pending yoksa bir kere yükle
  if (!m_actionsModified) {
    m_pendingActions = _macroman().getActions(activeMacro.id);
    m_actionsModified = true;
  }
  if (row < 0 || row >= m_pendingActions.size()) return;

  const MacroAction original = m_pendingActions[row];

  AdditionalSettingsDialog dialog(original, this);
  if (dialog.exec() == QDialog::Accepted) {
    MacroAction updated = dialog.getUpdatedAction();

    if (updated != original) {
      m_pendingActions[row] = updated;
      markRowAsModified(row);
    }
  }
}

void MainWindow::loadLanguage() {
  // TOOLBAR ACTION ÇEVİRİLERİ
  ui->actionAbout->setText(tr("about"));
  ui->actionSettings->setText(tr("settings"));
  ui->actionSave->setText(tr("macro save"));

  // headerlar
  ui->actionsTable->setHorizontalHeaderLabels(
      QStringList() << "No" << tr("action type") << tr("click type")
                    << tr("mouse button") << tr("repeat") << tr("interval")
                    << tr("additionals"));

  // tooltipler
  for (int i = 0; i < ui->actionsTable->columnCount(); ++i) {
    if (ui->actionsTable->horizontalHeaderItem(i)) {
      switch (i) {
        case 0:
          ui->actionsTable->horizontalHeaderItem(i)->setToolTip("No");
          break;
        case 1:
          ui->actionsTable->horizontalHeaderItem(i)->setToolTip(
              tr("action type tooltip"));
          break;
        case 2:
          ui->actionsTable->horizontalHeaderItem(i)->setToolTip(
              tr("click type tooltip"));
          break;
        case 3:
          ui->actionsTable->horizontalHeaderItem(i)->setToolTip(
              tr("mouse button tooltip"));
          break;
        case 4:
          ui->actionsTable->horizontalHeaderItem(i)->setToolTip(
              tr("repeat tooltip"));
          break;
        case 5:
          ui->actionsTable->horizontalHeaderItem(i)->setToolTip(
              tr("interval tooltip"));
          break;
        case 6:
          ui->actionsTable->horizontalHeaderItem(i)->setToolTip(
              tr("additionals tooltip"));
          break;
      }
    }
  }

  ui->actionActiveMacro->setToolTip(tr("active macro label tooltip"));
  ui->actionSave->setToolTip(tr("macro save tooltip"));
  ui->actionSettings->setToolTip(tr("settings tooltip"));
  ui->actionAbout->setToolTip(tr("about tooltip"));

  ui->btnAddAction->setToolTip(tr("add action tooltip"));
  ui->btnDeleteAction->setToolTip(tr("delete action tooltip"));
  ui->btnEditAction->setToolTip(tr("edit orders tooltip"));

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
  QString settingsPath = _appdataman().settingsFilePath();
  if (_settingsman().saveSettings(settingsPath, m_settings)) {
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

    _settingsman().saveSettings(
        _appdataman().settingsFilePath(), updatedSettings);

    // UI güncellemeleri
    // aktif makro
    std::optional<Macro> query = _macroman().getMacroById(
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

    QTimer::singleShot(10, this, [this]() {
      if (!m_isClosing) {
        MarkAsSaved();
      }
    });
  }
}

void MainWindow::refreshMacros() {
  m_macros = _macroman().getAllMacros();
  minfo() << "Macros refreshed. Count " << m_macros.size();
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
      hserr()
          << "Old macro's hotkey unregisteration failed after macro changing!";
      return;
    }
    // yeni makronun hotkeyini register et
    if (!hotkeyService->registerHotkey(htkStr, newActiveMacroId)) {
      hserr()
          << "New macro's hotkey registeration failed after macro changing!";
      return;
    }

    minfo() << "Active macro set to ID: " << newActiveMacroId;

    QTimer::singleShot(20, this, [this]() { MarkAsSaved(); });
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
    m_pendingActions = _macroman().getActions(activeMacro.id);
    m_actionsModified = true;
  }

  MacroAction a;
  a.action_type = ActionType::MOUSE;  // Default to MOUSE
  a.click_type = ClickType::CLICK;
  a.click_count = 1;
  a.current_pos = true;
  a.hold_duration = 1000;
  a.interval = 100;
  a.repeat = 0;
  a.position = "0, 0";
  a.macro_id = activeMacro.id;
  a.key_name = std::nullopt;

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
  m_hasUnsavedChanges = true;

  QString unsavedText =
      tr("active macro label").replace("#MCR", activeMacro.name) + "* (" +
      getHotkeyString(activeMacro.hotkey) + ")";

  ui->actionActiveMacro->setText(unsavedText);

  QFont f = ui->actionActiveMacro->font();
  f.setBold(true);
  ui->actionActiveMacro->setFont(f);
}

bool MainWindow::checkPointlessCycles() {
  QVector<MacroAction> acts;

  for (int row = 0; row < ui->actionsTable->rowCount(); ++row) {
    MacroAction a = getActionFromRow(row);
    acts.append(a);
  }

  if (acts.isEmpty()) return true;  // action yoksa sorun yok

  for (int i = 0; i < acts.size(); ++i) {
    const MacroAction& a = acts[i];

    // Mevcut sonsuz repeat kontrolü
    if (a.repeat == 0 && acts.size() > 1) {
      showmsg(tr("Warning: %1th action repeats infinitely, following actions "
                 "will never execute.")
                  .arg(i),
              MessageType::ERR);
      return false;
    }

    // Negatif interval kontrolü
    if (a.interval < 0) {
      showmsg(tr("Warning: Action %1 has negative interval (%2ms). Interval "
                 "must be >= 0.")
                  .arg(i + 1)
                  .arg(a.interval),
              MessageType::ERR);
      return false;
    }

    // Hold duration kontrolü
    if (a.hold_duration.has_value()) {
      if (a.hold_duration.value() < 0) {
        showmsg(tr("Warning: Action %1 has negative hold duration (%2ms). Hold "
                   "duration must be >= 0.")
                    .arg(i + 1)
                    .arg(a.hold_duration.value()),
                MessageType::ERR);
        return false;
      }
    }

    // HOLD action için hold_duration zorunlu
    if (a.click_type == ClickType::HOLD &&
        (!a.hold_duration.has_value() || a.hold_duration.value() == 0)) {
      showmsg(tr("Warning: Action %1 is HOLD type but has no hold duration. "
                 "HOLD actions require a positive hold duration.")
                  .arg(i + 1),
              MessageType::ERR);
      return false;
    }

    // Click count kontrolü
    if (a.click_count <= 0) {
      showmsg(tr("Warning: Action %1 has invalid click count (%2). Click count "
                 "must be positive.")
                  .arg(i + 1)
                  .arg(a.click_count),
              MessageType::ERR);
      return false;
    }

    // HOVER için click count 1'den fazla mantıksız
    if (a.click_type == ClickType::HOVER && a.click_count > 1) {
      showmsg(tr("Warning: Action %1 is HOVER type but has click count > 1. "
                 "HOVER actions should have click count = 1.")
                  .arg(i + 1),
              MessageType::ERR);
      return false;
    }

    // Mouse action için position kontrolü
    if (a.action_type == ActionType::MOUSE) {
      if (!a.current_pos.value_or(false) && !a.position.has_value()) {
        showmsg(tr("Warning: Action %1 is mouse action but has no position "
                   "specified and current_pos is false.")
                    .arg(i + 1),
                MessageType::ERR);
        return false;
      }

      // Position formatı kontrolü (varsa)
      if (a.position.has_value()) {
        QStringList coords = a.position.value().split(",");
        if (coords.size() != 2) {
          showmsg(tr("Warning: Action %1 has invalid position format. Expected "
                     "'x,y' format.")
                      .arg(i + 1),
                  MessageType::ERR);
          return false;
        }

        bool okX, okY;
        int x = coords[0].trimmed().toInt(&okX);
        int y = coords[1].trimmed().toInt(&okY);

        if (!okX || !okY) {
          showmsg(tr("Warning: Action %1 has invalid position coordinates. "
                     "Must be valid integers.")
                      .arg(i + 1),
                  MessageType::ERR);
          return false;
        }

        // Ekran sınırları kontrolü (isteğe bağlı)
        QRect screenGeometry = QApplication::primaryScreen()->geometry();
        if (x < 0 || y < 0 || x > screenGeometry.width() ||
            y > screenGeometry.height()) {
          showmsg(tr("Warning: Action %1 position (%2,%3) is outside screen "
                     "bounds.")
                      .arg(i + 1)
                      .arg(x)
                      .arg(y),
                  MessageType::ERR);
          return false;
        }
      }
    }

    // Keyboard action için key_name kontrolü
    if (a.action_type == ActionType::KEYBOARD) {
      if (!a.key_name.has_value() || a.key_name.value().isEmpty()) {
        showmsg(tr("Warning: Action %1 is keyboard action but has no key "
                   "specified.")
                    .arg(i + 1),
                MessageType::ERR);
        return false;
      }
    }

    // Çok büyük interval uyarısı (performans için)
    if (a.interval > 300000) {  // 5 dakikadan fazla
      showmsg(tr("Warning: Action %1 has very large interval (%2ms = %3 "
                 "seconds). This might cause long delays.")
                  .arg(i + 1)
                  .arg(a.interval)
                  .arg(a.interval / 1000.0, 0, 'f', 1),
              MessageType::ERR);
      return false;
    }

    // Çok büyük hold duration uyarısı
    if (a.hold_duration.has_value() &&
        a.hold_duration.value() > 60000) {  // 1 dakikadan fazla
      showmsg(tr("Warning: Action %1 has very large hold duration (%2ms = %3 "
                 "seconds).")
                  .arg(i + 1)
                  .arg(a.hold_duration.value())
                  .arg(a.hold_duration.value() / 1000.0, 0, 'f', 1),
              MessageType::ERR);
      return false;
    }

    // Çok büyük repeat count uyarısı
    if (a.repeat > 10000) {
      showmsg(tr("Warning: Action %1 has very large repeat count (%2). This "
                 "might cause long execution times.")
                  .arg(i + 1)
                  .arg(a.repeat),
              MessageType::ERR);
      return false;
    }

    // Çok büyük click count uyarısı
    if (a.click_count > 100) {
      showmsg(tr("Warning: Action %1 has very large click count (%2). This "
                 "might cause unexpected behavior.")
                  .arg(i + 1)
                  .arg(a.click_count),
              MessageType::ERR);
      return false;
    }
  }

  // Tüm kontroller başarılı
  ui->labelErrors->setVisible(false);
  return true;
}
void MainWindow::MarkAsSaved() {
  m_hasUnsavedChanges = false;
  m_modifiedRows.clear();
  m_actionsModified = false;
  m_pendingActions.clear();

  QString savedText =
      tr("active macro label").replace("#MCR", activeMacro.name) + " (" +
      getHotkeyString(activeMacro.hotkey) + ")";

  ui->actionActiveMacro->setText(savedText);

  QFont f = ui->actionActiveMacro->font();
  f.setBold(false);
  ui->actionActiveMacro->setFont(f);
}

void MainWindow::on_btnDeleteAction_clicked() {
  QList<QTableWidgetItem*> selected = ui->actionsTable->selectedItems();
  if (selected.isEmpty()) {
    showmsg(tr("select an action to del"), MessageType::WARN);
    return;
  }

  int row = selected.first()->row();

  // Eğer ilk kez değişiklik yapılıyorsa, mevcut actions'ları memory'e yükle
  if (!m_actionsModified) {
    m_pendingActions = _macroman().getActions(activeMacro.id);
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

  if (!checkPointlessCycles()) {
    return;
  }

  QString err;
  QVector<MacroAction> base =
      m_actionsModified ? m_pendingActions
                        : _macroman().getActions(activeMacro.id);

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
                               ? std::optional<MouseButton>(
                                     combo->currentData().value<MouseButton>())
                               : std::nullopt;
        }
      }
    }

    // repeat / interval
    if (auto it = ui->actionsTable->item(row, 4)) {
      bool ok;
      a.repeat = it->text().toInt(&ok);
      if (!ok) {
        showmsg(tr("Repeat value isn't an integer."), MessageType::ERR);
        return;
      }
    }
    if (auto it = ui->actionsTable->item(row, 5)) {
      bool ok;
      a.interval = it->text().toInt(&ok);
      if (!ok) {
        showmsg(tr("Integer value isn't an integer."), MessageType::ERR);
        return;
      }
    }

    // temel id/order
    a.macro_id = activeMacro.id;
    a.order = row;

    actionsToSave.append(a);
  }

  if (!_macroman().setActionsForMacro(activeMacro.id,
                                                   actionsToSave, &err)) {
    merr() << "Failed to save actions: " << err;
    showmsg(tr("Save failed: %1").arg(err), MessageType::ERR);
    _macroman().normalizeOrders(activeMacro.id, nullptr);
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

void MainWindow::on_btnEditAction_clicked() {
  if (m_reorderMode) {
    // Confirm Order - reorder mode'dan çık ve değişiklikleri koru
    exitReorderMode();

    // Unsaved changes olarak işaretle
    if (!m_hasUnsavedChanges) {
      m_hasUnsavedChanges = true;
      MarkAsUnsaved();
    }

    showmsg(tr("Action order updated. Don't forget to save your changes!"),
            MessageType::SUCCESS);

    // 3 saniye sonra mesajı temizle
    QTimer::singleShot(3000, this, [this]() {
      if (!m_reorderMode) {  // Hala reorder mode'da değilse temizle
        ui->labelErrors->setVisible(false);
        ui->labelErrors->setProperty("reorderMode", false);
        ui->labelErrors->setProperty("statusType", "");  // Clear property
        ui->labelErrors->style()->unpolish(ui->labelErrors);
        ui->labelErrors->style()->polish(ui->labelErrors);
      }
    });

    return;
  }

  // Normal mode - reorder mode'a gir
  QList<QTableWidgetItem*> selected = ui->actionsTable->selectedItems();
  if (selected.isEmpty()) {
    // Eğer seçim yoksa ilk satırı seç
    if (ui->actionsTable->rowCount() > 0) {
      ui->actionsTable->selectRow(0);
      enterReorderMode(0);
    } else {
      showmsg(tr("No actions to reorder"), MessageType::ERR);
      ui->labelErrors->setProperty("reorderMode", false);
    }
    return;
  }

  int currentRow = selected.first()->row();
  enterReorderMode(currentRow);
}
void MainWindow::enterReorderMode(int row) {
  m_reorderMode = true;
  m_reorderCurrentRow = row;
  m_reorderOriginalLabelText = ui->labelErrors->text();  // Orijinal metni sakla
  m_reorderOriginalLabelVisible =
      ui->labelErrors->isVisible();  // Orijinal visibility durumunu sakla

  // Pending actions'ı mevcut UI'dan oluştur (DB'den değil)
  if (!m_actionsModified) {
    // UI'dan current actions'ları çek
    QVector<MacroAction> currentActions;
    for (int i = 0; i < ui->actionsTable->rowCount(); ++i) {
      MacroAction action = getActionFromRow(i);
      currentActions.append(action);
    }
    m_pendingActions = currentActions;
    m_actionsModified = true;
  }

  // UI'yi reorder mode için ayarla
  ui->actionsTable->setFocusPolicy(Qt::StrongFocus);
  ui->actionsTable->setFocus();
  ui->actionsTable->selectRow(row);

  // Kullanıcıya bilgi ver - sürekli mode olduğunu belirt
  showmsg(tr("Use UP/DOWN arrows to move actions, select different rows to "
             "continue reordering. Click 'Confirm Order' to save changes or "
             "ESC to cancel"),
          MessageType::INFO);

  // Key event filter ekle
  ui->actionsTable->installEventFilter(this);

  // Buton metnini değiştir
  ui->btnEditAction->setText(tr("Confirm Order"));

  // Mouse click events'ini de handle etmek için
  connect(ui->actionsTable, &QTableWidget::cellClicked, this,
          &MainWindow::onReorderCellClicked, Qt::UniqueConnection);

  highlightReorderRow();
}

void MainWindow::onReorderCellClicked(int row, int column) {
  Q_UNUSED(column)
  if (m_reorderMode) {
    // Önceki satırın highlight'ını temizle
    clearRowHighlights();

    // Yeni satırı seç ve highlight et
    m_reorderCurrentRow = row;
    highlightReorderRow();

    // Label'ın gitmesini engelle - sadece row bilgisini güncelle
    showmsg(tr("Selected row %1. Use UP/DOWN arrows to move, select other rows "
               "to continue, 'Confirm Order' to save or ESC to cancel")
                .arg(row + 1),
            MessageType::INFO);
  }
}

void MainWindow::exitReorderMode() {
  if (!m_reorderMode) return;

  m_reorderMode = false;
  m_reorderCurrentRow = -1;

  // UI'yi normal mode'a döndür
  ui->actionsTable->removeEventFilter(this);

  // Cell click connection'ı kaldır
  disconnect(ui->actionsTable, &QTableWidget::cellClicked, this,
             &MainWindow::onReorderCellClicked);

  // Buton metnini geri al
  ui->btnEditAction->setText(tr("edit action orders"));

  // Orijinal label durumunu geri yükle
  if (m_reorderOriginalLabelVisible) {
    showmsg(m_reorderOriginalLabelText);
    ui->labelErrors->setVisible(true);
  } else {
    ui->labelErrors->setVisible(false);
  }
  ui->labelErrors->setProperty("reorderMode", false);
  ui->labelErrors->setProperty("statusType", "");  // Clear property

  clearRowHighlights();
}

void MainWindow::moveRowUp() {
  if (m_reorderCurrentRow <= 0) return;

  // Memory'de swap
  m_pendingActions.swapItemsAt(m_reorderCurrentRow, m_reorderCurrentRow - 1);

  // Order'ları güncelle
  m_pendingActions[m_reorderCurrentRow].order = m_reorderCurrentRow;
  m_pendingActions[m_reorderCurrentRow - 1].order = m_reorderCurrentRow - 1;

  // Current row'u güncelle
  m_reorderCurrentRow--;

  // Tabloyu tamamen yeniden inşa et (güvenli)
  rebuildTableFromPendingActions();

  // Seçimi güncelle
  ui->actionsTable->selectRow(m_reorderCurrentRow);
  highlightReorderRow();

  // Unsaved olarak işaretle
  if (!m_hasUnsavedChanges) {
    m_hasUnsavedChanges = true;
    MarkAsUnsaved();
  }
}

void MainWindow::moveRowDown() {
  if (m_reorderCurrentRow >= ui->actionsTable->rowCount() - 1) return;

  // Memory'de swap
  m_pendingActions.swapItemsAt(m_reorderCurrentRow, m_reorderCurrentRow + 1);

  // Order'ları güncelle
  m_pendingActions[m_reorderCurrentRow].order = m_reorderCurrentRow;
  m_pendingActions[m_reorderCurrentRow + 1].order = m_reorderCurrentRow + 1;

  // Current row'u güncelle
  m_reorderCurrentRow++;

  // Tabloyu tamamen yeniden inşa et (güvenli)
  rebuildTableFromPendingActions();

  // Seçimi güncelle
  ui->actionsTable->selectRow(m_reorderCurrentRow);
  highlightReorderRow();

  // Unsaved olarak işaretle
  if (!m_hasUnsavedChanges) {
    m_hasUnsavedChanges = true;
    MarkAsUnsaved();
  }
}

void MainWindow::rebuildTableFromPendingActions() {
  // Tabloyu temizle
  ui->actionsTable->setRowCount(0);

  // Pending actions'dan tabloyu yeniden inşa et
  for (const MacroAction& action : m_pendingActions) {
    addActionToTable(action);
  }
}

void MainWindow::highlightReorderRow() {
  // Önce tüm highlight'ları temizle
  clearRowHighlights();

  if (m_reorderCurrentRow >= 0 &&
      m_reorderCurrentRow < ui->actionsTable->rowCount()) {
    // Seçili satırı vurgula
    ui->actionsTable->selectRow(m_reorderCurrentRow);

    // Daha belirgin görsel feedback
    for (int col = 0; col < ui->actionsTable->columnCount(); ++col) {
      if (QTableWidgetItem* item =
              ui->actionsTable->item(m_reorderCurrentRow, col)) {
        // Gradient benzeri effect için farklı renk tonları
        item->setBackground(
            QBrush(QColor(70, 130, 255, 150)));  // Daha koyu mavi
        item->setForeground(
            QBrush(QColor(255, 255, 255)));  // Beyaz text daha iyi kontrast

        // Border effect
        QFont font = item->font();
        font.setBold(true);
        item->setFont(font);
      }

      // Widget'lar için de enhanced highlight
      if (QWidget* widget =
              ui->actionsTable->cellWidget(m_reorderCurrentRow, col)) {
        widget->setProperty("reorderHighlight", true);
      }
    }

    // Satır yüksekliğini biraz artır (daha dramatik görünüm için)
    ui->actionsTable->setRowHeight(m_reorderCurrentRow, 36);
  }
}

void MainWindow::clearRowHighlights() {
  // Tüm satırların background'ını ve formatting'ini temizle
  for (int row = 0; row < ui->actionsTable->rowCount(); ++row) {
    for (int col = 0; col < ui->actionsTable->columnCount(); ++col) {
      if (QTableWidgetItem* item = ui->actionsTable->item(row, col)) {
        item->setBackground(QBrush());
        item->setForeground(QBrush());  // Text rengini de sıfırla

        // Font'u da normal haline çevir
        QFont font = item->font();
        font.setBold(false);
        item->setFont(font);
      }

      // Widget'ların style'ını da temizle
      if (QWidget* widget = ui->actionsTable->cellWidget(row, col)) {
        // Reorder ile eklenen style'ları temizle
        widget->setProperty("reorderHighlight", "");
      }
    }

    // Satır yüksekliğini normale döndür
    ui->actionsTable->setRowHeight(row, 32);
  }
}
bool MainWindow::eventFilter(QObject* watched, QEvent* event) {
  if (watched == ui->actionsTable && m_reorderMode &&
      event->type() == QEvent::KeyPress) {
    QKeyEvent* keyEvent = static_cast<QKeyEvent*>(event);

    switch (keyEvent->key()) {
      case Qt::Key_Up:
        moveRowUp();
        return true;

      case Qt::Key_Down:
        moveRowDown();
        return true;

      case Qt::Key_Return:
      case Qt::Key_Enter:
        // Enter ile de confirm et
        on_btnEditAction_clicked();  // Confirm Order işlemi
        return true;

      case Qt::Key_Escape:
        // Cancel - orijinal sıralamayı yükle
        ui->labelErrors->setVisible(true);
        showmsg(tr("Reorder cancelled, changes discarded"), MessageType::INFO);

        // Orijinal data'yı yükle
        setActiveMacro(activeMacro.id);
        exitReorderMode();

        // 3 saniye sonra mesajı temizle
        QTimer::singleShot(3000, this, [this]() {
          if (!m_reorderMode) {
            ui->labelErrors->setVisible(false);
          }
        });
        return true;
    }
  }

  return QMainWindow::eventFilter(watched, event);
}
void MainWindow::showMoveAnimation(int fromRow, int toRow) {
  // From row
  for (int col = 0; col < ui->actionsTable->columnCount(); ++col) {
    if (QTableWidgetItem* item = ui->actionsTable->item(fromRow, col)) {
      item->setBackground(QBrush(QColor(255, 100, 100, 150)));
    }
    if (QWidget* widget = ui->actionsTable->cellWidget(fromRow, col)) {
      widget->setProperty("animationState", "from");
    }
  }

  // To row
  for (int col = 0; col < ui->actionsTable->columnCount(); ++col) {
    if (QTableWidgetItem* item = ui->actionsTable->item(toRow, col)) {
      item->setBackground(QBrush(QColor(100, 255, 100, 150)));
    }
    if (QWidget* widget = ui->actionsTable->cellWidget(toRow, col)) {
      widget->setProperty("animationState", "to");
    }
  }

  // Tabloyu repaint et
  ui->actionsTable->repaint();

  // Qt event loop'a işlem yaptır (animasyon etkisi için)
  QApplication::processEvents();

  // 150ms bekle
  QThread::msleep(150);

  // From row'u temizle
  for (int col = 0; col < ui->actionsTable->columnCount(); ++col) {
    if (QTableWidgetItem* item = ui->actionsTable->item(fromRow, col)) {
      item->setData(Qt::UserRole + 1, "");  // Clear custom property
    }
    if (QWidget* widget = ui->actionsTable->cellWidget(fromRow, col)) {
      widget->setProperty("animationState", "");  // Clear property
    }
  }

  // To row'u temizle
  for (int col = 0; col < ui->actionsTable->columnCount(); ++col) {
    if (QTableWidgetItem* item = ui->actionsTable->item(toRow, col)) {
      item->setData(Qt::UserRole + 1, "");  // Clear custom property
    }
    if (QWidget* widget = ui->actionsTable->cellWidget(toRow, col)) {
      widget->setProperty("animationState", "");  // Clear property
    }
  }

  // Son repaint
  ui->actionsTable->repaint();
}
