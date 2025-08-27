#ifndef HOTKEYSERVICE_H
#define HOTKEYSERVICE_H

#include <QAbstractNativeEventFilter>
#include <QMap>
#include <QObject>
#include <QString>
#include <QTimer>
#include <memory>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class HotkeyService : public QObject, public QAbstractNativeEventFilter {
  Q_OBJECT

 public:
  ~HotkeyService();

  struct HotkeyData {
    int id;
    QString keySequence;
    Qt::KeyboardModifiers modifiers;
    int key;
    bool isRegistered;
  };

  static HotkeyService* getInstance();
  static void destroyInstance();

  // Hotkey yönetimi
  bool registerHotkey(const QString& hotkeyString, int id);
  bool unregisterHotkey(int id);
  bool changeHotkey(int id, const QString& newHotkeyString);
  void unregisterAllHotkeys();

  // Utility fonksiyonları
  bool isHotkeyRegistered(int id) const;
  QString getHotkeyString(int id) const;
  QList<int> getRegisteredHotkeyIds() const;

  // Global hotkey string parsing
  static bool parseHotkeyString(const QString& hotkeyString,
                                Qt::KeyboardModifiers& modifiers, int& key);
  static QString formatHotkeyString(Qt::KeyboardModifiers modifiers, int key);

 protected:
  bool nativeEventFilter(const QByteArray& eventType, void* message,
                         qintptr* result) override;

 signals:
  void hotkeyPressed(int id);
  void hotkeyRegistrationFailed(int id, const QString& reason);

 private:
  explicit HotkeyService(QObject* parent = nullptr);

  // Singleton
  static std::unique_ptr<HotkeyService> m_instance;
  static bool m_destroyed;

  // Hotkey storage
  QMap<int, HotkeyData> m_registeredHotkeys;
  int m_nextHotkeyId;

  // Platform specific functions
  bool registerNativeHotkey(int id, Qt::KeyboardModifiers modifiers, int key);
  bool unregisterNativeHotkey(int id);

#ifdef Q_OS_WIN
  // Windows specific
  static Qt::Key virtualKeyToQt(DWORD vk);
  static DWORD qtKeyToVirtual(Qt::Key key);
  static UINT qtModifiersToNative(Qt::KeyboardModifiers modifiers);
#endif

  // Helper functions
  bool isValidHotkeyString(const QString& hotkeyString) const;
  QStringList getSupportedKeys() const;
  QStringList getSupportedModifiers() const;

  // Cleanup
  void cleanup();
};

#endif  // HOTKEYSERVICE_H
