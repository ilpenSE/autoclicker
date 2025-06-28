using AutoClicker.Properties;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Localization;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading;
using System.Windows.Forms;

namespace AutoClicker
{
    public partial class MainMenu : Form
    {
        private string DEFAULT_LANGUAGE = "en";
        private bool isWaitingForHotkey = false;

        public MainMenu()
        {
            InitializeComponent();
            Select();

            // set language by default lang on OS in first run
            if (Settings.Default.FirstRun)
            {
                CultureInfo ci = CultureInfo.CurrentUICulture;
                string defLang = ci.Name.Substring(0, 2);

                string[] availableLangs = new string[] { "tr", "en", "de", "fr" };
                ChangeLanguage(availableLangs.Contains(defLang) ? defLang : DEFAULT_LANGUAGE);
            }


            ClickTimer.Tick += TimerTick;

            RegisterGlobalHotkey();
            LoadLanguage();
            Settings.Default.FirstRun = false;
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            // Load values
            repeatTimes.Value = Settings.Default.RepeatTimes;
            irvHrs.Value = Settings.Default.IrvHrs;
            irvMins.Value = Settings.Default.IrvMins;
            irvSecs.Value = Settings.Default.IrvSecs;
            irvMillis.Value = Settings.Default.IrvMillis;
            langBtn.SelectedIndex = Settings.Default.LanguageIndex;
            // lang indexes: 0 = en, 1 = tr, 2 = de, 3 = fr

            posX.Maximum = decimal.MaxValue;
            posY.Maximum = decimal.MaxValue;
            posX.Value = Settings.Default.PosX;
            posY.Value = Settings.Default.PosY;

            bool isCurrentPos = Settings.Default.CurrentPos;
            currentPosBtn.Checked = isCurrentPos;
            specPosBtn.Checked = !isCurrentPos;

            bool isRepeatForever = Settings.Default.RepeatForever;
            repeatForeverBtn.Checked = isRepeatForever;
            repeatTimesBtn.Checked = !isRepeatForever;

            msButtonBtn.SelectedIndex = Settings.Default.MouseBtnIndex;
            clickTypeBtn.SelectedIndex = Settings.Default.ClickTypeIndex;

            Keys htk = (Keys)Settings.Default.StartHotkey;
            startBtn.Text = Resources.start + " (" + htk.ToString() + ")";
            stopBtn.Text = Resources.stop + " (" + htk.ToString() + ")";
        }

        #region LANGUAGE

        private void ChangeLanguage(string lang)
        {
            Settings.Default.LanguageIndex = GetLanguageIndex(lang);
            langBtn.SelectedIndex = GetLanguageIndex(lang);
            LoadLanguage();
        }

        private int GetLanguageIndex(string lang)
        {
            switch (lang)
            {
                case "en":
                    return 0;
                case "tr":
                    return 1;
                case "de":
                    return 2;
                case "fr":
                    return 3;
                default:
                    return -1;
            }
        }

        private string GetLanguageByIndex(int index)
        {
            switch (index)
            {
                case 0:
                    return "en";
                case 1:
                    return "tr";
                case 2:
                    return "de";
                case 3:
                    return "fr";
                default:
                    return null;
            }
        }

        private string GetLanguage()
        {
            switch (Settings.Default.LanguageIndex)
            {
                case 0:
                    return "en";
                case 1:
                    return "tr";
                case 2:
                    return "de";
                case 3:
                    return "fr";
                default:
                    return null;
            }
        }

        private IStringLocalizer<Resources> GetLocalizer()
        {
            // this returns localizer which has localized texts
            var services = new ServiceCollection();
            services.AddLogging();
            services.AddLocalization(options => options.ResourcesPath = "Properties");

            var provider = services.BuildServiceProvider();

            // set the culture
            CultureInfo.CurrentUICulture = new CultureInfo(GetLanguage());

            // return localizer
            return provider.GetRequiredService<IStringLocalizer<Resources>>();
        }

        private void LoadLanguage()
        {
            // change the texts
            var localizer = GetLocalizer();

            // interval
            interval.Text = Resources.interval_groupbox;
            hrslbl.Text = Resources.interval_hours;
            minlbl.Text = Resources.interval_mins;
            seclbl.Text = Resources.interval_secs;
            millislbl.Text = Resources.interval_millis;

            // position
            position.Text = Resources.pos_gb;
            currentPosBtn.Text = Resources.pos_current;
            specPosBtn.Text = Resources.pos_spec;

            // repeat
            repeat.Text = Resources.rep_gb;
            repeatForeverBtn.Text = Resources.rep_forever;
            repeatTimesBtn.Text = Resources.rep_gb;
            timeslbl.Text = Resources.rep_times;

            // options
            options.Text = Resources.opt_gb;
            clicktypelbl.Text = Resources.opt_clicktype;
            msbtnlbl.Text = Resources.opt_msbtn;
            langlbl.Text = Resources.opt_lang;

            // click type
            clickTypeBtn.Items[0] = Resources.opt_ct_single;
            clickTypeBtn.Items[1] = Resources.opt_ct_double;
            clickTypeBtn.Items[2] = Resources.opt_ct_triple;
            clickTypeBtn.Items[3] = Resources.opt_ct_quadruple;

            // mouse button
            msButtonBtn.Items[0] = Resources.opt_mb_left;
            msButtonBtn.Items[1] = Resources.opt_mb_right;
            msButtonBtn.Items[2] = Resources.opt_mb_mid;

            // buttons
            Keys htkCode = (Keys)Settings.Default.StartHotkey;
            setHtkBtn.Text = Resources.sethtk;
            cancelSettingHotkey.Text = Resources.cancel_setting_htk;
            cancelSettingHotkey.Location = new Point((this.Size.Width - cancelSettingHotkey.Size.Width) / 2, cancelSettingHotkey.Location.Y);
            startBtn.Text = Resources.start + " (" + htkCode.ToString() + ")";
            stopBtn.Text = Resources.stop + " (" + htkCode.ToString() + ")";
            this.Text = Resources.title;
        }

        #endregion

        #region VALUE CHANGE LISTENERS

        private void langBtn_SelectedIndexChanged(object sender, EventArgs e)
        {
            ChangeLanguage(GetLanguageByIndex(langBtn.SelectedIndex));
        }
        private void irvHrs_ValueChanged(object sender, EventArgs e)
        {
            Settings.Default.IrvHrs = (int)irvHrs.Value;
        }

        private void irvMins_ValueChanged(object sender, EventArgs e)
        {
            Settings.Default.IrvMins = (int)irvMins.Value;
        }

        private void irvSecs_ValueChanged(object sender, EventArgs e)
        {
            Settings.Default.IrvSecs = (int)irvSecs.Value;
        }

        private void irvMillis_ValueChanged(object sender, EventArgs e)
        {
            Settings.Default.IrvMillis = (int)irvMillis.Value;
        }

        private void currentPosBtn_CheckedChanged(object sender, EventArgs e)
        {
            Settings.Default.CurrentPos = currentPosBtn.Checked;
        }

        private void posX_ValueChanged(object sender, EventArgs e)
        {
            Settings.Default.PosX = (int)posX.Value;
        }

        private void posY_ValueChanged(object sender, EventArgs e)
        {
            Settings.Default.PosY = (int)posY.Value;
        }

        private void repeatForeverBtn_CheckedChanged(object sender, EventArgs e)
        {
            Settings.Default.RepeatForever = repeatForeverBtn.Checked;
        }

        private void repeatTimes_ValueChanged(object sender, EventArgs e)
        {
            Settings.Default.RepeatTimes = (int)repeatTimes.Value;
        }

        private void msButtonBtn_SelectedIndexChanged(object sender, EventArgs e)
        {
            Settings.Default.MouseBtnIndex = msButtonBtn.SelectedIndex;
        }

        private void clickTypeBtn_SelectedIndexChanged(object sender, EventArgs e)
        {
            Settings.Default.ClickTypeIndex = clickTypeBtn.SelectedIndex;
        }
        #endregion

        private void startBtn_Click(object sender, EventArgs e)
        {
            if (!isWaitingForHotkey)
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
            // checking interval
            int interval = GetIntervalInMillis();
            if (interval <= 0)
            {
                interval = 1;
            }

            // checking position
            Rectangle screen = Screen.FromControl(this).Bounds;
            if (!currentPosBtn.Checked ? posX.Value > screen.Width || posY.Value > screen.Height : false)
            {
                MessageBox.Show(Resources.err_invalid_pos, Resources.err_title, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            // starting clicker
            isClicking = true;
            startBtn.Enabled = false;
            stopBtn.Enabled = true;
            ClickTimer.Start();
            ClickTimer.Interval = interval;
        }

        private void StopAutoClicker()
        {
            ClickTimer.Stop();
            isClicking = false;
            startBtn.Enabled = true;
            stopBtn.Enabled = false;
            repeats = 0;
        }

        private void setHtkBtn_Click(object sender, EventArgs e)
        {
            isWaitingForHotkey = true;
            this.KeyPreview = true; // Form'un klavye tuşlarını almasını sağlar
            setHtkBtn.Text = Resources.waiting;
            cancelSettingHotkey.Visible = true;
            this.Text = Resources.title_waiting;
        }

        private void RegisterGlobalHotkey()
        {
            UnregisterHotKey(this.Handle, HOTKEY_ID); // önce eskisini kaldır

            var key = (Keys)Settings.Default.StartHotkey;
            var modifiers = Settings.Default.HotkeyModifiers;

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
                    MessageBox.Show(Resources.err_assignmodifier_txt, Resources.err_title, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                Settings.Default.StartHotkey = (int)e.KeyCode;
                Settings.Default.HotkeyModifiers = modifiers;
                Settings.Default.Save();

                startBtn.Text = Resources.start + " (" + e.KeyCode.ToString() + ")";
                stopBtn.Text = Resources.stop + " (" + e.KeyCode.ToString() + ")";

                MessageBox.Show(Resources.done_htkset_txt.Replace("#HTK", e.KeyCode.ToString()), "Auto Clicker", MessageBoxButtons.OK, MessageBoxIcon.Information);

                isWaitingForHotkey = false;
                setHtkBtn.Text = Resources.sethtk;
                cancelSettingHotkey.Visible = false;
                this.Text = Resources.title;
                RegisterGlobalHotkey();
            }
        }

        protected override void WndProc(ref Message m)
        {
            const int WM_HOTKEY = 0x0312;
            if (m.Msg == WM_HOTKEY && m.WParam.ToInt32() == HOTKEY_ID && !isWaitingForHotkey)
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
            setHtkBtn.Text = Resources.sethtk;
            this.Text = Resources.title;
        }


        private void cancelSettingHotkey_MouseEnter(object sender, EventArgs e)
        {
            cancelSettingHotkey.Font = new Font("Microsoft Sans Serif", 12.0f, FontStyle.Underline, GraphicsUnit.Point, (byte)(0));
        }

        private void cancelSettingHotkey_MouseLeave(object sender, EventArgs e)
        {
            cancelSettingHotkey.Font = new Font("Microsoft Sans Serif", 12.0f, FontStyle.Regular, GraphicsUnit.Point, (byte)(0));
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
            int msbtn = msButtonBtn.SelectedIndex;
            int clickType = (int)clickTypeBtn.SelectedIndex + 1;

            uint downCode = 0;
            uint upCode = 0;

            switch (msbtn)
            {
                case 0:
                    downCode = MOUSEEVENTF_LEFTDOWN;
                    upCode = MOUSEEVENTF_LEFTUP;
                    break;
                case 1:
                    downCode = MOUSEEVENTF_RIGHTDOWN;
                    upCode = MOUSEEVENTF_RIGHTUP;
                    break;
                case 2:
                    downCode = MOUSEEVENTF_MIDDLEDOWN;
                    upCode = MOUSEEVENTF_MIDDLEUP;
                    break;
                default:
                    MessageBox.Show("Invalid mouse button!");
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
