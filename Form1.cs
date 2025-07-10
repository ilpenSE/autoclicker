using AutoClicker.Properties;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Localization;
using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Forms;

namespace AutoClicker
{
    public partial class MainMenu : Form
    {
        private ClickerEngine _clicker;
        private HotkeyService _hotkey;
        private SettingsModel _settings;
        private Dictionary<string, MacroModel> _Macros;
        private string _activeMacroName = "DEFAULT";
        private bool isModified = false;

        private bool isClicking = false;
        private bool isWaitingForHotkey = false;
        private System.Windows.Forms.Timer ClickTimer = new System.Windows.Forms.Timer();
        private int repeats = 0;

        public MainMenu()
        {
            InitializeComponent();

            _settings = SettingsManager.Load();

            _Macros = MacroManager.LoadAll();

            if (!_Macros.ContainsKey("DEFAULT"))
            {
                MessageBox.Show(Resources.wrn_cantfounddef, Resources.wrn_title, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                MacroManager.AddDefaults();
            }

            _activeMacroName = _settings.ActiveMacro;

            // check macro is valid
            if (!_Macros.ContainsKey(_activeMacroName))
            {
                // MACRO NOT FOUND, SET TO DEFAULT
                _activeMacroName = "DEFAULT";
                _settings.ActiveMacro = "DEFAULT";
                SettingsManager.Save(_settings);
            }

            LoadMacroFormettings(_activeMacroName);
            UpdateMacroLabel();


            if (Enum.TryParse(_settings.Hotkey, out Keys parsedKey))
            {
                _hotkey = new HotkeyService(this.Handle, parsedKey, ToggleClicker);
            }
            else
            {
                _hotkey = new HotkeyService(this.Handle, SettingsManager.DefaultKey, ToggleClicker);
                _settings.Hotkey = SettingsManager.DefaultKey.ToString();
                SettingsManager.Save(_settings);
                MessageBox.Show(Resources.wrn_invalidhtk, Resources.wrn_title, MessageBoxButtons.OK, MessageBoxIcon.Warning);
            }

            // after set the settings
            Select();
            if (_settings.FirstRun)
            {
                string defLang = CultureInfo.CurrentUICulture.Name.Substring(0, 2);

                Language lang;
                switch (defLang)
                {
                    case "en":
                        lang = Language.ENGLISH;
                        break;
                    case "tr":
                        lang = Language.TURKISH;
                        break;
                    case "de":
                        lang = Language.GERMAN;
                        break;
                    case "fr":
                        lang = Language.FRENCH;
                        break;
                    default:
                        lang = SettingsManager.DefaultLanguage;
                        break;
                }

                ChangeLanguage(lang);
            }


            ClickTimer.Tick += TimerTick;

            // check the language index is valid
            if (_settings.LanguageIndex > (langBtn.Items.Count - 1) || _settings.LanguageIndex < 0)
            {
                ChangeLanguage(SettingsManager.DefaultLanguage);
                _settings.LanguageIndex = (int)SettingsManager.DefaultLanguage;
                SettingsManager.Save(_settings);
            }

            LoadLanguage();
            langBtn.SelectedIndex = (int)GetLanguage();
            _settings.FirstRun = false;

            this.KeyPreview = true;

            // add listeners
            this.msButtonBtn.SelectedIndexChanged += new System.EventHandler(this.msButtonBtn_SelectedIndexChanged);
            this.clickTypeBtn.SelectedIndexChanged += new System.EventHandler(this.clickTypeBtn_SelectedIndexChanged);

            RegisterChangeListeners();
        }

        private void RegisterChangeListeners()
        {
            this.msButtonBtn.SelectedIndexChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.clickTypeBtn.SelectedIndexChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.posY.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.posX.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.specPosBtn.CheckedChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.currentPosBtn.CheckedChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.irvMillis.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.irvSecs.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.irvMins.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.irvHrs.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.repeatTimes.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.repeatForeverBtn.CheckedChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.repeatTimesBtn.CheckedChanged += new System.EventHandler(this.MarkAsUnsaved);
        }

        private void MarkAsUnsaved(object sender, EventArgs e)
        {
            if (!isModified)
            {
                activemacrolbl.Font = new Font(activemacrolbl.Font, FontStyle.Bold);
                activemacrolbl.Text = Resources.activemacro_unsaved.Replace("{_activeMacroName}", _activeMacroName);
                isModified = true;
            }
        }

        private void UpdateMacroLabel()
        {
            activemacrolbl.Text = Resources.activemacro.Replace("{_activeMacroName}", (_activeMacroName == "DEFAULT" ? "Default" : _activeMacroName));
            activemacrolbl.Font = new Font(activemacrolbl.Font, FontStyle.Regular);
        }

        private void LoadMacroFormettings(string macroName)
        {
            try
            {
                if (!_Macros.TryGetValue(macroName, out var macro)) return;

                posX.Value = macro.Position.X;
                posY.Value = macro.Position.Y;
                currentPosBtn.Checked = macro.Position.CurrentPosition;
                specPosBtn.Checked = !macro.Position.CurrentPosition;

                try { irvMillis.Value = macro.Interval % 1000; }
                catch (Exception ex) { MessageBox.Show("irvMillis → " + ex.Message); }

                try { irvSecs.Value = (macro.Interval / 1000) % 60; }
                catch (Exception ex) { MessageBox.Show("irvSecs → " + ex.Message); }

                try { irvMins.Value = (macro.Interval / 60000) % 60; }
                catch (Exception ex) { MessageBox.Show("irvMins → " + ex.Message); }

                try { irvHrs.Value = (macro.Interval / 3600000); }
                catch (Exception ex) { MessageBox.Show("irvHrs → " + ex.Message); }


                repeatForeverBtn.Checked = macro.RepeatTimes.RepeatForever;
                repeatTimesBtn.Checked = !macro.RepeatTimes.RepeatForever;
                repeatTimes.Value = macro.RepeatTimes.Count;

                msButtonBtn.SelectedIndex = (int)macro.MouseButton;
                clickTypeBtn.SelectedIndex = (int)macro.ClickType;

                _clicker = new ClickerEngine();
                _clicker.SetInterval(macro.Interval);
                _clicker.SetClickCount((int)macro.ClickType + 1);
                _clicker.SetButton(macro.MouseButton);
            } catch (NullReferenceException e)
            {
                Console.WriteLine(macroName);
                throw e;
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
            CultureInfo.CurrentUICulture = new CultureInfo(GetLanguageCode(GetLanguage()));

            // return localizer
            return provider.GetRequiredService<IStringLocalizer<Resources>>();
        }

        private void setHtkBtn_Click(object sender, EventArgs e)
        {
            if (isClicking)
                StopAutoClicker();

            if (isModified)
                SaveCurrentMacro();

            if (isWaitingForHotkey)
            {
                isWaitingForHotkey = false;
                this.KeyPreview = false;
                setHtkBtn.Text = Resources.sethtk;
                this.Text = Resources.title;
            } else
            {
                isWaitingForHotkey = true;
                this.KeyPreview = true;
                setHtkBtn.Text = Resources.cancel;
                this.Text = Resources.title_waiting;
            }
        }



        private void startBtn_Click(object sender, EventArgs e)
        {
            if (!isWaitingForHotkey) { ToggleClicker(); }
        }


        private void stopBtn_Click(object sender, EventArgs e)
        {
            ToggleClicker();
        }

        private void SaveCurrentMacro()
        {
            if (!isModified) return;

            var macro = new MacroModel
            {
                Name = _activeMacroName,
                MouseButton = (MouseButton)msButtonBtn.SelectedIndex,
                ClickType = (ClickType)clickTypeBtn.SelectedIndex,
                Interval = GetIntervalInMillis(),
                Position = new Position
                {
                    X = (int)posX.Value,
                    Y = (int)posY.Value,
                    CurrentPosition = currentPosBtn.Checked
                },
                RepeatTimes = new RepeatTimes
                {
                    Count = (int)repeatTimes.Value,
                    RepeatForever = repeatForeverBtn.Checked
                }
            };

            if (_activeMacroName == "DEFAULT")
            {
                string newName = Interaction.InputBox(Resources.ask_enternewmacro, Resources.ask_savemacrotitle);
                if (string.IsNullOrWhiteSpace(newName)) return;

                macro.Name = newName;
                MacroManager.SaveMacro(newName, macro);
                _activeMacroName = newName;
            }
            else
            {
                MacroManager.SaveMacro(_activeMacroName, macro);
            }

            _Macros[macro.Name] = macro;

            // UI Güncelle
            isModified = false;
            activemacrolbl.Text = Resources.activemacro.Replace("{_activeMacroName}", _activeMacroName);
            activemacrolbl.Font = new Font(activemacrolbl.Font, FontStyle.Regular);
        }

        private void MainMenu_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.Control && e.KeyCode == Keys.S)
            {
                SaveCurrentMacro();
                e.SuppressKeyPress = true;
            }

            if (isWaitingForHotkey)
            {
                uint modifiers = 0;
                if (e.Control) modifiers |= 0x0002; // MOD_CONTROL
                if (e.Alt) modifiers |= 0x0001;     // MOD_ALT
                if (e.Shift) modifiers |= 0x0004;   // MOD_SHIFT

                if (e.KeyCode == Keys.ControlKey || e.KeyCode == Keys.ShiftKey || e.KeyCode == Keys.Menu)
                {
                    MessageBox.Show(Resources.err_assignmodifier_txt, Resources.err_title, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                startBtn.Text = Resources.start + " (" + e.KeyCode.ToString() + ")";
                stopBtn.Text = Resources.stop + " (" + e.KeyCode.ToString() + ")";

                _settings.Hotkey = e.KeyCode.ToString();
                SettingsManager.Save(_settings);

                setHtkBtn.Text = Resources.sethtk;
                this.Text = Resources.title;
                _hotkey.Dispose();
                _hotkey = new HotkeyService(this.Handle, e.KeyCode, ToggleClicker);

                MessageBox.Show(Resources.done_htkset_txt.Replace("#HTK", e.KeyCode.ToString()), "Auto Clicker", MessageBoxButtons.OK, MessageBoxIcon.Information);

                isWaitingForHotkey = false;
            }
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
            Keys htkCode;
            if (!Enum.TryParse(_settings.Hotkey, out htkCode))
            {
                htkCode = SettingsManager.DefaultKey;
            }

            setHtkBtn.Text = Resources.sethtk;
            startBtn.Text = Resources.start + " (" + htkCode.ToString() + ")";
            stopBtn.Text = Resources.stop + " (" + htkCode.ToString() + ")";
            this.Text = Resources.title;
        }

        private void ToggleClicker()
        {
            if (isClicking)
                StopAutoClicker();
            else
                StartAutoClicker();

            if (isModified)
                SaveCurrentMacro();
        }

        private int GetIntervalInMillis()
        {
            int total = 0;
            total += (int)irvMillis.Value;
            total += ((int)irvSecs.Value) * 1000;
            total += ((int)irvMins.Value) * 60 * 1000;
            total += ((int)irvHrs.Value) * 3600 * 1000;
            return total;
        }

        private void StartAutoClicker()
        {
            int interval = GetIntervalInMillis();
            if (interval <= 0) interval = 1;

            isClicking = true;
            startBtn.Enabled = false;
            stopBtn.Enabled = true;

            _clicker.SetInterval(interval);
            _clicker.Start();

            ClickTimer.Interval = interval;
            ClickTimer.Start();
        }

        private void StopAutoClicker()
        {
            _clicker.Stop();
            ClickTimer.Stop();
            isClicking = false;
            startBtn.Enabled = true;
            stopBtn.Enabled = false;
            repeats = 0;
        }

        private void TimerTick(object sender, EventArgs e)
        {
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
                SetCursorPos((int)posX.Value, (int)posY.Value);
                Thread.Sleep(10);
            }
        }

        private void MainMenu_FormClosing(object sender, FormClosingEventArgs e)
        {
            _clicker.Stop();
            _hotkey.Dispose();
            _settings.Hotkey = _hotkey.GetHotkey().ToString();
            _settings.LanguageIndex = langBtn.SelectedIndex;
            _settings.ActiveMacro = _activeMacroName;

            SettingsManager.Save(_settings);
        }

        private void ChangeLanguage(Language lang)
        {
            langBtn.SelectedIndex = (int)lang;
            CultureInfo.CurrentUICulture = new CultureInfo(GetLanguageCode(lang));
        }

        public static string GetLanguageCode(Language lang)
        {
            switch (lang)
            {
                case Language.ENGLISH:
                    return "en";
                case Language.TURKISH:
                    return "tr";
                case Language.GERMAN:
                    return "de";
                case Language.FRENCH:
                    return "fr";
                default:
                    return GetLanguageCode(SettingsManager.DefaultLanguage);
            }
        }


        public static Language GetLanguageByIndex(int index)
        {
            if (Enum.TryParse(index.ToString(), out Language res))
            {
                return res;
            } else
            {
                return SettingsManager.DefaultLanguage;
            }

        }

        private Language GetLanguage()
        {
            return GetLanguageByIndex(_settings.LanguageIndex);
        }

        [DllImport("user32.dll")]
        static extern bool SetCursorPos(int X, int Y);

        private void msButtonBtn_SelectedIndexChanged(object sender, EventArgs e)
        {
            _clicker.SetButton((MouseButton)msButtonBtn.SelectedIndex);
        }

        private void clickTypeBtn_SelectedIndexChanged(object sender, EventArgs e)
        {
            _clicker.SetClickCount(clickTypeBtn.SelectedIndex + 1);
        }

        private void langBtn_SelectedIndexChanged(object sender, EventArgs e)
        {
            _settings.LanguageIndex = langBtn.SelectedIndex;
            LoadLanguage();
            isModified = false;
            UpdateMacroLabel();
        }

        private void activemacrolbl_Click(object sender, EventArgs e)
        {
            using(var macroForm = new MacroForm(_Macros, _activeMacroName))
            {
                if (macroForm.ShowDialog() == DialogResult.OK)
                {
                    _settings.ActiveMacro = macroForm.SelectedMacroName;
                    _activeMacroName = macroForm.SelectedMacroName;

                    _Macros = MacroManager.LoadAll();

                    SettingsManager.Save(_settings);
                    LoadMacroFormettings(_activeMacroName);
                    UpdateMacroLabel();

                    isModified = false;
                    activemacrolbl.Font = new Font(activemacrolbl.Font, FontStyle.Regular);

                }
            }


        }

        private void MainMenu_Load(object sender, EventArgs e)
        {
            LoadMacroFormettings(_activeMacroName);

            this.DoubleBuffered = true;
            this.SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint | ControlStyles.OptimizedDoubleBuffer, true);

            foreach (GroupBox gb in new GroupBox[] {interval, position, repeat, options})
            {
                gb.Paint += groupBox_Paint;
            }

        }

        private void groupBox_Paint(object sender, PaintEventArgs e)
        {
            GroupBox box = (GroupBox)sender;
            e.Graphics.Clear(box.BackColor);

            // Renk ayarları
            Color borderColor = Color.FromArgb(220, 220, 220); // Açık gri
            Color textColor = Color.White;

            using (Pen pen = new Pen(borderColor))
            {
                SizeF textSize = e.Graphics.MeasureString(box.Text, box.Font);
                int textHeight = (int)textSize.Height;
                int textWidth = (int)textSize.Width;
                int offset = 10; // Sol boşluk

                // Dikdörtgenin kenarlarını çiz (üst hariç)
                Rectangle rect = new Rectangle(0, textHeight / 2, box.Width - 1, box.Height - textHeight / 2 - 1);

                // Sol çizgi
                e.Graphics.DrawLine(pen, rect.Left, rect.Top, rect.Left, rect.Bottom);
                // Alt çizgi
                e.Graphics.DrawLine(pen, rect.Left, rect.Bottom, rect.Right, rect.Bottom);
                // Sağ çizgi
                e.Graphics.DrawLine(pen, rect.Right, rect.Bottom, rect.Right, rect.Top);
                // Üst çizgi (iki parçalı, yazının etrafı boş kalacak)
                e.Graphics.DrawLine(pen, rect.Left, rect.Top, offset - 2, rect.Top); // yazı öncesi
                e.Graphics.DrawLine(pen, offset + textWidth + 2, rect.Top, rect.Right, rect.Top); // yazı sonrası

                // Başlık yazısını çiz
                using (Brush brush = new SolidBrush(textColor))
                {
                    PointF textPos = new PointF(offset, 0);
                    e.Graphics.DrawString(box.Text, box.Font, brush, textPos);
                }
            }
        }


        public static string CapitalizeFirstLetter(string input)
        {
            if (string.IsNullOrWhiteSpace(input))
                return string.Empty;

            input = input.Trim();
            return char.ToUpper(input[0]) + input.Substring(1).ToLower();
        }

        private void activemacrolbl_MouseEnter(object sender, EventArgs e)
        {
            activemacrolbl.Font = new Font(activemacrolbl.Font, isModified ? FontStyle.Bold : FontStyle.Underline);
        }

        private void activemacrolbl_MouseLeave(object sender, EventArgs e)
        {
            activemacrolbl.Font = new Font(activemacrolbl.Font, isModified ? FontStyle.Bold : FontStyle.Regular);
        }
    }
}