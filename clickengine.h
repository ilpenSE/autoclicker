#ifndef CLICKENGINE_H
#define CLICKENGINE_H

#include <QObject>
#include <QTimer>
#include <QPoint>
#include <QList>
#include <QMap>
#include <memory>
#include "Enums.h"

// Forward declarations
struct Macro;
struct MacroAction;

#ifdef Q_OS_WIN
#include <windows.h>
#endif

class ClickEngine : public QObject
{
    Q_OBJECT

public:
    ~ClickEngine();

    struct ExecutionState {
        bool isRunning;
        int currentActionIndex;
        int currentRepeatCount;
        int totalActionsExecuted;
        int currentCycleCount;

        ExecutionState() : isRunning(false), currentActionIndex(0),
            currentRepeatCount(0), totalActionsExecuted(0), currentCycleCount(0) {}
    };

    static ClickEngine* getInstance();
    static void destroyInstance();

    // Single macro execution (only one can run at a time)
    bool startMacro(int macroId);
    bool stopCurrentMacro();
    bool pauseCurrentMacro();
    bool resumeCurrentMacro();

    // Status queries
    bool isMacroRunning() const;
    int getCurrentMacroId() const;
    ExecutionState getCurrentMacroState() const;

    // Action execution
    void performAction(const MacroAction& action);
    void performMouseAction(const MacroAction& action);
    void performKeyboardAction(const MacroAction& action);

    // Utility
    QPoint getCurrentMousePosition() const;
    void setMousePosition(const QPoint& position);

    // Helper functions for position parsing
    static QPoint parsePosition(const QString& positionStr);
    static QString formatPosition(const QPoint& position);

signals:
    void macroStarted(int macroId);
    void macroStopped(int macroId);
    void macroPaused(int macroId);
    void macroResumed(int macroId);
    void actionExecuted(int macroId, int actionIndex, int totalExecuted);
    void macroError(int macroId, const QString& error);
    void cycleCompleted(int macroId, int cycleCount);

private:
    explicit ClickEngine(QObject* parent = nullptr);

    // Singleton
    static std::unique_ptr<ClickEngine> m_instance;
    static bool m_destroyed;

    // Single macro execution state
    int m_currentMacroId;
    ExecutionState m_executionState;
    QTimer* m_executionTimer;
    QList<MacroAction> m_currentActions;

    // Execution helpers
    void executeMacroStep();
    void executeCurrentAction();
    void scheduleNextAction();
    void moveToNextAction();
    void completeCurrentCycle();
    QList<MacroAction> loadMacroActions(int macroId);

    // Platform specific mouse operations
    void nativeMouseClick(const QPoint& pos, MouseButton button, int count = 1);
    void nativeMousePress(const QPoint& pos, MouseButton button);
    void nativeMouseRelease(const QPoint& pos, MouseButton button);
    void nativeMouseMove(const QPoint& pos);
    void nativeMouseHover(const QPoint& pos, int duration);
    void nativeMouseHold(const QPoint& pos, MouseButton button, int duration);

    // Platform specific keyboard operations
    void nativeKeyPress(const QString& key);
    void nativeKeyRelease(const QString& key);
    void nativeKeyClick(const QString& key, int count = 1);
    void nativeKeyHold(const QString& key, int duration);

    // Conversion helpers
    QString mouseButtonToString(MouseButton button) const;
    MouseButton stringToMouseButton(const QString& buttonStr) const;

#ifdef Q_OS_WIN
    // Windows specific
    DWORD mouseButtonToWin32(MouseButton button, bool isPress) const;
    WORD qtKeyToVirtualKey(const QString& key) const;
#endif

    // Cleanup
    void cleanup();

private slots:
    void onExecutionTimer();
};

#endif // CLICKENGINE_H
