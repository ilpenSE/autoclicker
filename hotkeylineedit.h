#ifndef HOTKEYLINEEDIT_H
#define HOTKEYLINEEDIT_H

#include <QLineEdit>
#include <QKeyEvent>
#include <QSet>
#include <QTimer>
#include <QLabel>
#include <QMouseEvent>

class HotkeyLineEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit HotkeyLineEdit(QWidget *parent = nullptr);
    ~HotkeyLineEdit();

    // Hotkey set/get
    void setHotkey(const QString& hotkey);
    QString getHotkey() const;

    // Capture kontrol
    void startCapture();
    void stopCapture();
    bool isCapturing() const { return m_capturing; }

    // Kullanıcıya yönelik yardımcılar
    void acceptHotkey();   // finalize + clearFocus
    void cancelCapture();  // iptal edip eski değeri geri koy

signals:
    void hotkeyReady(const QString& hotkey);
    void hotkeyChanged();

protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void focusInEvent(QFocusEvent* event) override;
    void focusOutEvent(QFocusEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;

private slots:
    void updateDisplay();

private:
    QString keyToString(int key) const;
    bool isModifierKey(int key) const;
    bool isValidKey(int key) const;
    QString formatCurrentHotkey() const;
    void clearCapture();
    void finalizeCapturedHotkey();

    QSet<int> m_pressedKeys;
    QSet<int> m_modifiers;
    int m_mainKey;
    bool m_capturing;
    QString m_finalHotkey;
    QTimer* m_updateTimer;
    QLabel* m_dummyFocusWidget; // invisible dummy focus widget

    // Safety flags
    bool m_inUpdateDisplay;
    bool m_inFocusEvent;
};

#endif // HOTKEYLINEEDIT_H
