using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

public class HotkeyService : IMessageFilter, IDisposable
{
    private int _hotkeyId = Guid.NewGuid().GetHashCode() & 0x7FFFFFFF; // a positive ID
    private Keys _hotkey { get; set; }
    private readonly int _modifiers;
    private readonly Action _onHotkeyPressed;
    private readonly IntPtr _handle;
    private bool _disposed = false;

    [DllImport("user32.dll")]
    private static extern bool RegisterHotKey(IntPtr hWnd, int id, int fsModifiers, int vk);

    [DllImport("user32.dll")]
    private static extern bool UnregisterHotKey(IntPtr hWnd, int id);

    public HotkeyService(IntPtr handle, Keys hotkey, Action onHotkeyPressed, int modifiers = 0)
    {
        _handle = handle;
        _hotkey = hotkey;
        _modifiers = modifiers;
        _onHotkeyPressed = onHotkeyPressed;

        Application.AddMessageFilter(this);
        bool success = RegisterHotKey(_handle, _hotkeyId, _modifiers, (int)_hotkey);
        if (!success)
        {
            Console.WriteLine($"The id {_hotkeyId} was in use by other program! Changing it.");
            _hotkeyId = GenerateRandomId();
        }
        Console.WriteLine($"Hotkey ID: {_hotkeyId}");
    }

    private int GenerateRandomId()
    {
        return new Random().Next(2, int.MaxValue);
    }

    public Keys Hotkey => _hotkey;

    public bool PreFilterMessage(ref Message m)
    {
        const int WM_HOTKEY = 0x0312;
        if (m.Msg == WM_HOTKEY && (int)m.WParam == _hotkeyId)
        {
            _onHotkeyPressed?.Invoke();
            return true;
        }
        return false;
    }

    public Keys GetHotkey()
    {
        return _hotkey;
    }

    public void SetHotkey(Keys hotkey)
    {
        this._hotkey = hotkey;
    }

    public static int ConvertModifiersToFsModifiers(Keys modifiers)
    {
        int fsModifiers = 0;

        if (modifiers.HasFlag(Keys.Control)) fsModifiers |= 0x0002;
        if (modifiers.HasFlag(Keys.Shift)) fsModifiers |= 0x0004;
        if (modifiers.HasFlag(Keys.Alt)) fsModifiers |= 0x0001;

        return fsModifiers;
    }


    public void Dispose()
    {
        if (_disposed) return;

        UnregisterHotKey(_handle, _hotkeyId);
        Application.RemoveMessageFilter(this);

        _disposed = true;
    }
}
