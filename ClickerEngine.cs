using System;
using System.Runtime.InteropServices;
using System.Threading;
using System.Threading.Tasks;

public class ClickerEngine
{
    [DllImport("user32.dll")]
    private static extern void mouse_event(int dwFlags, int dx, int dy, int dwData, int dwExtraInfo);

    private const int MOUSEEVENTF_LEFTDOWN = 0x02;
    private const int MOUSEEVENTF_LEFTUP = 0x04;
    private const int MOUSEEVENTF_RIGHTDOWN = 0x08;
    private const int MOUSEEVENTF_RIGHTUP = 0x10;
    private const int MOUSEEVENTF_MIDDLEDOWN = 0x20;
    private const int MOUSEEVENTF_MIDDLEUP = 0x40;

    private CancellationTokenSource _cts;
    private int _interval;
    private int _clickCount = 1;
    private MouseButton _button = MouseButton.LEFT;

    public bool IsRunning => _cts != null && !_cts.IsCancellationRequested;

    public ClickerEngine()
    {
        
    }

    public void SetInterval(int ms) => _interval = ms;

    public void SetClickCount(int count) => _clickCount = Math.Max(1, count);

    public void SetButton(MouseButton button) => _button = button;

    public void Start()
    {
        if (IsRunning) return;
        _cts = new CancellationTokenSource();
        Task.Run(() => RunClickLoop(_cts.Token));
    }

    public void Stop() => _cts?.Cancel();

    private async Task RunClickLoop(CancellationToken token)
    {
        while (!token.IsCancellationRequested)
        {
            Click();
            await Task.Delay(_interval, token);
        }
    }

    private void Click()
    {
        int down = MOUSEEVENTF_LEFTDOWN, up = MOUSEEVENTF_LEFTUP;
        switch (_button)
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

        for (int i = 0; i < _clickCount; i++)
        {
            mouse_event(down, 0, 0, 0, 0);
            mouse_event(up, 0, 0, 0, 0);
        }
    }
}