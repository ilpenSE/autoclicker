using System;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;

public class ClickerEngine
{
    [DllImport("user32.dll")]
    private static extern void mouse_event(int dwFlags, int dx, int dy, int dwData, int dwExtraInfo);

    [DllImport("user32.dll")]
    private static extern bool SetCursorPos(int X, int Y);

    private const int MOUSEEVENTF_LEFTDOWN = 0x02;
    private const int MOUSEEVENTF_LEFTUP = 0x04;
    private const int MOUSEEVENTF_RIGHTDOWN = 0x08;
    private const int MOUSEEVENTF_RIGHTUP = 0x10;
    private const int MOUSEEVENTF_MIDDLEDOWN = 0x20;
    private const int MOUSEEVENTF_MIDDLEUP = 0x40;

    private CancellationTokenSource _cts;
    private MacroModel _macro;

    public bool IsRunning => _cts != null && !_cts.IsCancellationRequested;

    public ClickerEngine() { }

    public void SetMacro(MacroModel macro) => _macro = macro;
    public void Start()
    {
        if (IsRunning) return;
        _cts = new CancellationTokenSource();
        Task.Run(() => RunClickLoop(_cts.Token));
    }

    public void Stop() => _cts?.Cancel();

    private async Task RunClickLoop(CancellationToken token)
    {
        if (_macro.ClickMode == ClickMod.CLICK)
        {
            while (!token.IsCancellationRequested)
            {
                Click();
                await Task.Delay(_macro.Interval, token);
            }
        }
        else if (_macro.ClickMode == ClickMod.HOLD)
        {
            while (!token.IsCancellationRequested)
            {
                MouseDown();
                try
                {
                    await Task.Delay(_macro.HoldDuration, token);
                }
                catch (TaskCanceledException)
                {
                    // even if task cancelled, mouse up have to be called
                }
                finally
                {
                    MouseUp();
                }

                try
                {
                    await Task.Delay(_macro.Interval, token);
                }
                catch (TaskCanceledException)
                {
                    // loop ends here
                    break;
                }
            }
        }

    }
    private void Click()
    {
        int down = MOUSEEVENTF_LEFTDOWN, up = MOUSEEVENTF_LEFTUP;
        switch (_macro.MouseButton)
        {
            case MouseButton.RIGHT:
                down = MOUSEEVENTF_RIGHTDOWN;
                up = MOUSEEVENTF_RIGHTUP;
                break;
            case MouseButton.MIDDLE:
                down = MOUSEEVENTF_MIDDLEDOWN;
                up = MOUSEEVENTF_MIDDLEUP;
                break;
        }

        for (int i = 0; i < (int)_macro.ClickType+1; i++)
        {
            mouse_event(down, 0, 0, 0, 0);
            mouse_event(up, 0, 0, 0, 0);
        }
    }

    private void MouseDown()
    {
        int down = MOUSEEVENTF_LEFTDOWN;

        switch (_macro.MouseButton)
        {
            case MouseButton.RIGHT:
                down = MOUSEEVENTF_RIGHTDOWN;
                break;
            case MouseButton.MIDDLE:
                down = MOUSEEVENTF_MIDDLEDOWN;
                break;
        }

        mouse_event(down, 0, 0, 0, 0);
    }

    private void MouseUp()
    {
        int up = MOUSEEVENTF_LEFTUP;

        switch (_macro.MouseButton)
        {
            case MouseButton.RIGHT:
                up = MOUSEEVENTF_RIGHTUP;
                break;
            case MouseButton.MIDDLE:
                up = MOUSEEVENTF_MIDDLEUP;
                break;
        }

        mouse_event(up, 0, 0, 0, 0);
    }
}
