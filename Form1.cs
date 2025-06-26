using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AutoClicker
{
    public partial class MainMenu : Form
    {

        private bool isWaitingForHotkey = false;

        public MainMenu()
        {
            InitializeComponent();
            Select();

            ClickTimer.Tick += TimerTick;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Load values
            repeatTimes.Value = Properties.Settings.Default.RepeatTimes;
            posX.Value = Properties.Settings.Default.PosX;
            posY.Value = Properties.Settings.Default.PosY;
            irvHrs.Value = Properties.Settings.Default.IrvHrs;
            irvMins.Value = Properties.Settings.Default.IrvMins;
            irvSecs.Value = Properties.Settings.Default.IrvSecs;
            irvMillis.Value = Properties.Settings.Default.IrvMillis;

            bool isCurrentPos = Properties.Settings.Default.CurrentPos;
            currentPosBtn.Checked = isCurrentPos;
            specPosBtn.Checked = !isCurrentPos;

            bool isRepeatForever = Properties.Settings.Default.RepeatForever;
            repeatForeverBtn.Checked = isRepeatForever;
            repeatTimesBtn.Checked = !isRepeatForever;

            msButtonBtn.SelectedIndex = Properties.Settings.Default.MouseBtnIndex;
            clickTypeBtn.SelectedIndex = Properties.Settings.Default.ClickTypeIndex;

            Keys htk = (Keys)Properties.Settings.Default.StartHotkey;
            startBtn.Text = "START (" + htk.ToString() + ")";
            stopBtn.Text = "STOP (" + htk.ToString() + ")";

            RegisterGlobalHotkey();
        }

        #region VALUE CHANGE LISTENERS
        private void irvHrs_ValueChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default.IrvHrs = (int)irvHrs.Value;
        }

        private void irvMins_ValueChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default.IrvMins = (int)irvMins.Value;
        }

        private void irvSecs_ValueChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default.IrvSecs = (int)irvSecs.Value;
        }

        private void irvMillis_ValueChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default.IrvMillis = (int)irvMillis.Value;
        }

        private void currentPosBtn_CheckedChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default.CurrentPos = currentPosBtn.Checked;
        }

        private void posX_ValueChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default.PosX = (int)posX.Value;
        }

        private void posY_ValueChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default.PosY = (int)posY.Value;
        }

        private void repeatForeverBtn_CheckedChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default.RepeatForever = repeatForeverBtn.Checked;
        }

        private void repeatTimes_ValueChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default.RepeatTimes = (int)repeatTimes.Value;
        }

        private void msButtonBtn_SelectedIndexChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default.MouseBtnIndex = msButtonBtn.SelectedIndex;
        }

        private void clickTypeBtn_SelectedIndexChanged(object sender, EventArgs e)
        {
            Properties.Settings.Default.ClickTypeIndex = clickTypeBtn.SelectedIndex;
        }
        #endregion

        private void startBtn_Click(object sender, EventArgs e)
        {
            ToggleClicker();
        }

        private bool isClicking = false;

        private void ToggleClicker()
        {
            if (isClicking)
                StopAutoClicker();
            else
                StartAutoClicker();
        }

        private int GetIntervalInMillis()
        {
            int interval = 0;

            interval += (int)irvMillis.Value;
            interval += ((int)irvSecs.Value) * 1000;
            interval += ((int)irvMins.Value) * 60 * 1000;
            interval += ((int)irvHrs.Value) * 3600 * 1000;

            return interval;
        }

        private System.Windows.Forms.Timer ClickTimer = new System.Windows.Forms.Timer();

        private int repeats = 0;

        private void StartAutoClicker()
        {
            if (GetIntervalInMillis() <= 0)
            {
                MessageBox.Show("Interval can't be 0!", "Auto Clicker", MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            isClicking = true;
            startBtn.Enabled = false;
            stopBtn.Enabled = true;
            ClickTimer.Start();
            ClickTimer.Interval = GetIntervalInMillis();
        }

        private void StopAutoClicker()
        {
            ClickTimer.Stop();
            isClicking = false;
            startBtn.Enabled = true;
            stopBtn.Enabled = false;
        }

        private void setHtkBtn_Click(object sender, EventArgs e)
        {
            isWaitingForHotkey = true;
            this.KeyPreview = true; // Form'un klavye tuşlarını almasını sağlar
            setHtkBtn.Text = "WAITING...";
            cancelSettingHotkey.Visible = true;
        }

        private void RegisterGlobalHotkey()
        {
            UnregisterHotKey(this.Handle, HOTKEY_ID); // önce eskisini kaldır

            var key = (Keys)Properties.Settings.Default.StartHotkey;
            var modifiers = Properties.Settings.Default.HotkeyModifiers;

            if (key != Keys.None)
            {
                RegisterHotKey(this.Handle, HOTKEY_ID, modifiers, key);
            }
        }


        private void MainMenu_KeyDown(object sender, KeyEventArgs e)
        {
            if (isWaitingForHotkey)
            {
                // Modifier (CTRL, SHIFT vb.) ayıkla
                uint modifiers = 0;
                if (e.Control) modifiers |= MOD_CONTROL;
                if (e.Alt) modifiers |= MOD_ALT;
                if (e.Shift) modifiers |= MOD_SHIFT;

                // Eğer sadece modifier tuşlarından birine basıldıysa uyarı ver
                if (e.KeyCode == Keys.ControlKey || e.KeyCode == Keys.ShiftKey || e.KeyCode == Keys.Menu) // Menu = Alt
                {
                    MessageBox.Show("You can't assign Ctrl, Shift and Alt.", "Invalid Key", MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    isWaitingForHotkey = false;
                    return;
                }

                Properties.Settings.Default.StartHotkey = (int)e.KeyCode;
                Properties.Settings.Default.HotkeyModifiers = modifiers;
                Properties.Settings.Default.Save();

                startBtn.Text = "START (" + e.KeyCode.ToString() + ")";
                stopBtn.Text = "STOP (" + e.KeyCode.ToString() + ")";

                MessageBox.Show("Hotkey set: " + e.KeyCode.ToString(), "Auto Clicker", MessageBoxButtons.OK, MessageBoxIcon.Information);

                isWaitingForHotkey = false;
                setHtkBtn.Text = "SET HOTKEY";
                cancelSettingHotkey.Visible = false;
                RegisterGlobalHotkey();
            }
        }

        protected override void WndProc(ref Message m)
        {
            const int WM_HOTKEY = 0x0312;
            if (m.Msg == WM_HOTKEY && m.WParam.ToInt32() == HOTKEY_ID)
            {
                // Hotkey'e basıldı!
                ToggleClicker();
            }
            base.WndProc(ref m);
        }


        private void label10_Click(object sender, EventArgs e)
        {
            cancelSettingHotkey.Visible = false;
            isWaitingForHotkey = false;
            setHtkBtn.Text = "SET HOTKEY";
        }


        private void cancelSettingHotkey_MouseEnter(object sender, EventArgs e)
        {
            cancelSettingHotkey.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Underline, GraphicsUnit.Point, (byte)(0));
        }

        private void cancelSettingHotkey_MouseLeave(object sender, EventArgs e)
        {
            cancelSettingHotkey.Font = new Font("Microsoft Sans Serif", 8.25f, FontStyle.Regular, GraphicsUnit.Point, (byte)(0));
        }

        // Windows API'yi çağırmak için
        [DllImport("user32.dll")]
        private static extern bool RegisterHotKey(IntPtr hWnd, int id, uint fsModifiers, Keys vk);

        [DllImport("user32.dll")]
        private static extern bool UnregisterHotKey(IntPtr hWnd, int id);

        // MOUSE EVENT
        [DllImport("user32.dll")]
        static extern void mouse_event(uint dwFlags, uint dx, uint dy, uint dwData, int dwExtraInfo);

        [DllImport("user32.dll")]
        static extern bool SetCursorPos(int X, int Y);

        private const uint MOUSEEVENTF_LEFTDOWN = 0x0002;
        private const uint MOUSEEVENTF_LEFTUP = 0x0004;
        private const uint MOUSEEVENTF_RIGHTDOWN = 0x0008;
        private const uint MOUSEEVENTF_RIGHTUP = 0x0010;
        private const uint MOUSEEVENTF_MIDDLEDOWN = 0x0020;
        private const uint MOUSEEVENTF_MIDDLEUP = 0x0040;

        // Modifier flags
        private const uint MOD_NONE = 0x0000;
        private const uint MOD_ALT = 0x0001;
        private const uint MOD_CONTROL = 0x0002;
        private const uint MOD_SHIFT = 0x0004;
        private const uint MOD_WIN = 0x0008;

        private const int HOTKEY_ID = 9000; // rastgele bir ID

        private void TimerTick(object sender, EventArgs e)
        {
            string msbtn = msButtonBtn.SelectedItem.ToString();
            int clickType = (int)clickTypeBtn.SelectedIndex + 1;

            uint downCode = 0;
            uint upCode = 0;

            switch (msbtn)
            {
                case "Left":
                    downCode = MOUSEEVENTF_LEFTDOWN;
                    upCode = MOUSEEVENTF_LEFTUP;
                    break;
                case "Right":
                    downCode = MOUSEEVENTF_RIGHTDOWN;
                    upCode = MOUSEEVENTF_RIGHTUP;
                    break;
                case "Mid":
                    downCode = MOUSEEVENTF_MIDDLEDOWN;
                    upCode = MOUSEEVENTF_MIDDLEUP;
                    break;
                default:
                    MessageBox.Show("Invalid click type!");
                    return;
            }

            if (!repeatForeverBtn.Checked)
            {
                repeats++;

                if (repeats == (int)repeatTimes.Value)
                {
                    StopAutoClicker();
                }
            }

            if (!currentPosBtn.Checked)
            {
                // Belirli pozisyona git
                SetCursorPos((int)posX.Value, (int)posY.Value);
                Thread.Sleep(10); // Pozisyon otursun diye kısa bir bekleme
            }

            for (int i = 0; i < clickType; i++)
            {
                mouse_event(downCode | upCode, 0, 0, 0, 0);
                // Thread.Sleep(10); // Çoklu tıklamalar arasında küçük gecikme
            }
        }


        private void MainMenu_FormClosing(object sender, FormClosingEventArgs e)
        {
            UnregisterHotKey(this.Handle, HOTKEY_ID);
        }

        private void stopBtn_Click(object sender, EventArgs e)
        {
            ToggleClicker();
        }
    }
}
