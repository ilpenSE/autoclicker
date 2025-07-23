using AutoClicker.Properties;
using Krypton.Toolkit;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Localization;
using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Globalization;
using System.Linq;
using System.Reflection;
using System.Runtime;
using System.Runtime.InteropServices;
using System.Threading;
using System.Windows.Controls;
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
        private int supportedLanguageCount = 4;

        private bool isClicking = false;
        private bool isWaitingForHotkey = false;
        private System.Windows.Forms.Timer ClickTimer = new System.Windows.Forms.Timer();
        private int repeats = 0;

        public static string VERSION = "v1.3";

        public MainMenu()
        {
            InitializeComponent();

            // load settings and macros
            _settings = SettingsManager.Load();
            _Macros = MacroManager.Macros;

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

            // load macro
            LoadMacroFormettings(_activeMacroName);
            UpdateMacroLabel();

            // load hotkey
            try
            {
                var (key, modifiers) = ConvertToKeySafe(_settings.Hotkey);
                int fsModifiers = HotkeyService.ConvertModifiersToFsModifiers(modifiers);

                _hotkey = new HotkeyService(this.Handle, key, ToggleClicker, fsModifiers);

                Console.WriteLine("Hotkey: " + ConvertToKeySafe(_settings.Hotkey));
            } catch (ArgumentException)
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
            if (_settings.LanguageIndex > (supportedLanguageCount - 1) || _settings.LanguageIndex < 0)
            {
                ChangeLanguage(SettingsManager.DefaultLanguage);
                _settings.LanguageIndex = (int)SettingsManager.DefaultLanguage;
                SettingsManager.Save(_settings);
            }

            LoadLanguage();
            _settings.FirstRun = false;

            this.KeyPreview = true;

            RegisterChangeListeners();

            SettingsManager.Save(_settings);
        }

        private void RegisterChangeListeners()
        {
            this.msButtonBtn.SelectedIndexChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.clickTypeBtn.SelectedIndexChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.cmodBtn.SelectedIndexChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.posY.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.posX.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.hdMillis.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.specPosBtn.CheckedChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.currentPosBtn.CheckedChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.irvMillis.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.repeatTimes.ValueChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.repeatForeverBtn.CheckedChanged += new System.EventHandler(this.MarkAsUnsaved);
            this.repeatTimesBtn.CheckedChanged += new System.EventHandler(this.MarkAsUnsaved);
        }

        private void MarkAsUnsaved(object sender, EventArgs e)
        {
            if (!isModified)
            {
                activemacrolbl.Font = FontLoader.GetBold(12f);
                activemacrolbl.Text = Resources.activemacro_unsaved.Replace("{_activeMacroName}", _activeMacroName);
                isModified = true;
            }
        }

        private void UpdateMacroLabel()
        {
            activemacrolbl.Text = Resources.activemacro.Replace("{_activeMacroName}", (_activeMacroName == "DEFAULT" ? Resources.defaultmacro : _activeMacroName));
            activemacrolbl.Font = FontLoader.GetRegular(12f);
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

                Console.WriteLine(macro.HoldDuration);
                try { hdMillis.Value = macro.HoldDuration; }
                catch (Exception ex) { Console.WriteLine("hdMillis → " + ex.Message); }

                try { irvMillis.Value = macro.Interval; }
                catch (Exception ex) { Console.WriteLine("irvMillis → " + ex.Message); }

                repeatForeverBtn.Checked = macro.RepeatTimes.RepeatForever;
                repeatTimesBtn.Checked = !macro.RepeatTimes.RepeatForever;
                repeatTimes.Value = macro.RepeatTimes.Count;

                msButtonBtn.SelectedIndex = (int)macro.MouseButton;
                clickTypeBtn.SelectedIndex = (int)macro.ClickType;
                cmodBtn.SelectedIndex = (int)macro.ClickMode;

                _clicker = new ClickerEngine();
                _clicker.SetMacro(macro);
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
                Description = MacroManager.GetMacroByName(_activeMacroName).Description,
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
                },
                ClickMode = (ClickMod)cmodBtn.SelectedIndex,
                HoldDuration = (int)hdMillis.Value
            };

            if (_activeMacroName == "DEFAULT")
            {
                string newName = Interaction.InputBox(Resources.ask_enternewmacro, Resources.ask_savemacrotitle);
                if (string.IsNullOrWhiteSpace(newName)) return;

                macro.Name = newName;
                macro.Description = Resources.createdmacro;
                MacroManager.SaveMacro(newName, macro);
                _activeMacroName = newName;
            }
            else
            {
                MacroManager.SaveMacro(_activeMacroName, macro);
            }

            _Macros[macro.Name] = macro;
            _clicker.SetMacro(macro);

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
        }

        private void LoadLanguage()
        {
            // change the texts
            var localizer = GetLocalizer();

            // interval
            interval.Text = Resources.interval_groupbox;
            irv_millislbl.Text = Resources.interval_millis;

            // hold duration
            holddur.Text = Resources.holddur_groupbox;
            hd_millislbl.Text = Resources.interval_millis;

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
            cmodelbl.Text = Resources.opt_cmod;

            // click type
            clickTypeBtn.Items[0] = Resources.opt_ct_single;
            clickTypeBtn.Items[1] = Resources.opt_ct_double;
            clickTypeBtn.Items[2] = Resources.opt_ct_triple;
            clickTypeBtn.Items[3] = Resources.opt_ct_quadruple;

            // mouse button
            msButtonBtn.Items[0] = Resources.opt_mb_left;
            msButtonBtn.Items[1] = Resources.opt_mb_right;
            msButtonBtn.Items[2] = Resources.opt_mb_mid;

            // click mod
            cmodBtn.Items[0] = Resources.opt_cm_click;
            cmodBtn.Items[1] = Resources.opt_cm_hold;

            // buttons
            settingsBtn.Text = Resources.settings_title.ToUpper(CultureInfo.GetCultureInfo("en-US"));
            startBtn.Text = Resources.start + " (" + _settings.Hotkey + ")";
            stopBtn.Text = Resources.stop + " (" + _settings.Hotkey + ")";
            this.Text = Resources.title;
            irvInfo.Text = Resources.interval_info;
            UpdateMacroLabel();
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
            return (int)irvMillis.Value;
        }

        private int GetHoldDurationInMillis()
        {
            return (int)hdMillis.Value;
        }

        private void StartAutoClicker()
        {
            int interval = GetIntervalInMillis();
            if (interval <= 0) interval = 1;

            int holdDuration = GetHoldDurationInMillis();
            if (holdDuration <= 0) holdDuration = 1;

            bool isHoldMode = cmodBtn.SelectedIndex == 1;

            isClicking = true;
            startBtn.Enabled = false;
            stopBtn.Enabled = true;

            _clicker.Start();

            if (!currentPosBtn.Checked)
            {
                SetCursorPos((int)posX.Value, (int)posY.Value);
            }

            ClickTimer.Interval = isHoldMode ? (interval + holdDuration) : interval;
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
            }
        }

        private void MainMenu_FormClosing(object sender, FormClosingEventArgs e)
        {
            _clicker.Stop();
            _hotkey.Dispose();
            _settings.ActiveMacro = _activeMacroName;

            SettingsManager.Save(_settings);
        }

        private void ChangeLanguage(Language lang)
        {
            _settings.LanguageIndex = (int)lang;
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

        private void activemacrolbl_Click(object sender, EventArgs e)
        {
            using(var macroForm = new MacroForm(_Macros, _activeMacroName))
            {
                if (macroForm.ShowDialog() == DialogResult.OK)
                {
                    _settings.ActiveMacro = macroForm.SelectedMacroName;
                    _activeMacroName = macroForm.SelectedMacroName;

                    _Macros = MacroManager.Macros;

                    SettingsManager.Save(_settings);
                    LoadMacroFormettings(_activeMacroName);
                    UpdateMacroLabel();

                    isModified = false;
                    activemacrolbl.Font = new Font(activemacrolbl.Font, FontStyle.Regular);

                }
            }
        }

        public static readonly Font ButtonFont = FontLoader.GetBold(10.5f);

        private void MainMenu_Load(object sender, EventArgs e)
        {
            LoadMacroFormettings(_activeMacroName);

            // some painting shit
            this.DoubleBuffered = true;
            this.SetStyle(ControlStyles.AllPaintingInWmPaint | ControlStyles.UserPaint | ControlStyles.OptimizedDoubleBuffer, true);

            // paint group boxes
            foreach (System.Windows.Forms.GroupBox gb in new System.Windows.Forms.GroupBox[] {interval, position, repeat, options, holddur})
            {
                gb.Paint += groupBox_Paint;
                gb.FlatStyle = FlatStyle.Standard;
                gb.ForeColor = Color.Transparent;
            }

            this.Font = FontLoader.GetRegular(9f);

            startBtn.StateCommon.Content.ShortText.Font = ButtonFont;
            startBtn.Click += new System.EventHandler(this.startBtn_Click);

            stopBtn.StateCommon.Content.ShortText.Font = ButtonFont;
            stopBtn.Click += new System.EventHandler(this.stopBtn_Click);

            settingsBtn.StateCommon.Content.ShortText.Font = ButtonFont;
            settingsBtn.Click += new System.EventHandler(this.settingsBtn_Click);

            SetDisabledStatesOfButtons();
            RegisterNumericFocusEvents();
            RegisterDropdownFocusEvents();
        }

        public static Color focusBorderClr = Color.FromArgb(70, 120, 200);
        public static Color normalBorderClr = Color.FromArgb(100, 100, 100);

        private void RegisterDropdownFocusEvents()
        {
            foreach (KryptonComboBox box in new KryptonComboBox[] {cmodBtn, msButtonBtn, clickTypeBtn})
            {
                box.MouseEnter += (s, ev) =>
                {
                    box.StateActive.ComboBox.Border.Color1 = focusBorderClr;
                };

                box.MouseLeave += (s, ev) =>
                {
                    box.StateActive.ComboBox.Border.Color1 = normalBorderClr;
                };

                box.GotFocus += (s, ev) =>
                {
                    box.StateActive.ComboBox.Border.Color1 = focusBorderClr;
                };

                box.LostFocus += (s, ev) =>
                {
                    box.StateActive.ComboBox.Border.Color1 = normalBorderClr;
                };
            }
        }

        private void RegisterNumericFocusEvents()
        {
            foreach (KryptonNumericUpDown kryptonNumericUpDown in new KryptonNumericUpDown[] { irvMillis, hdMillis, repeatTimes, posX, posY })
            {
                kryptonNumericUpDown.MouseEnter += (s, ev) =>
                {
                    if (!kryptonNumericUpDown.Focused)
                        kryptonNumericUpDown.StateCommon.Border.Color1 = focusBorderClr;
                };

                kryptonNumericUpDown.MouseLeave += (s, ev) =>
                {
                    if (!kryptonNumericUpDown.Focused)
                        kryptonNumericUpDown.StateCommon.Border.Color1 = normalBorderClr;
                };

                kryptonNumericUpDown.GotFocus += (s, ev) =>
                {
                    kryptonNumericUpDown.StateCommon.Border.Color1 = focusBorderClr;
                };

                kryptonNumericUpDown.LostFocus += (s, ev) =>
                {
                    kryptonNumericUpDown.StateCommon.Border.Color1 = normalBorderClr;
                };
            }
        }

        private void SetDisabledStatesOfButtons()
        {
            stopBtn.StateDisabled.Back.Color1 = Color.FromArgb(240, 200, 200);
            stopBtn.StateDisabled.Back.Color2 = Color.FromArgb(220, 180, 180);
            stopBtn.StateDisabled.Border.Color1 = Color.FromArgb(200, 160, 160);
            stopBtn.StateDisabled.Border.Color2 = Color.FromArgb(200, 160, 160);

            startBtn.StateDisabled.Back.Color1 = Color.FromArgb(200, 240, 200);
            startBtn.StateDisabled.Back.Color2 = Color.FromArgb(180, 220, 180);
            startBtn.StateDisabled.Border.Color1 = Color.FromArgb(150, 190, 150);
            startBtn.StateDisabled.Border.Color2 = Color.FromArgb(150, 190, 150);

            stopBtn.StateDisabled.Border.Rounding = 5;
            stopBtn.StateDisabled.Border.Width = 2;

            startBtn.StateDisabled.Border.Rounding = 5;
            startBtn.StateDisabled.Border.Width = 2;
        }

        private void groupBox_Paint(object sender, PaintEventArgs e)
        {
            System.Windows.Forms.GroupBox box = (System.Windows.Forms.GroupBox)sender;
            e.Graphics.Clear(box.BackColor);

            // Yumuşatma ayarları
            e.Graphics.SmoothingMode = SmoothingMode.AntiAlias;
            e.Graphics.TextRenderingHint = System.Drawing.Text.TextRenderingHint.ClearTypeGridFit;

            // Font ve renkler
            Font labelFont = FontLoader.GetBold(10f); // kendi font loader fonksiyonun
            Color borderColor = Color.FromArgb(220, 220, 220); // Açık gri
            Color textColor = Color.White;

            string text = box.Text;
            int offset = 10;

            // Metin ölçümü
            Size textSize = TextRenderer.MeasureText(text, labelFont);
            int textHeight = textSize.Height;
            int textWidth = textSize.Width;

            Rectangle rect = new Rectangle(0, textHeight / 2, box.Width - 1, box.Height - textHeight / 2 - 1);

            using (Pen pen = new Pen(borderColor, 1))
            {
                // Border çizgileri
                e.Graphics.DrawLine(pen, rect.Left, rect.Top, offset - 2, rect.Top); // sol taraf
                e.Graphics.DrawLine(pen, offset + textWidth + 2, rect.Top, rect.Right, rect.Top); // sağ taraf
                e.Graphics.DrawLine(pen, rect.Left, rect.Top, rect.Left, rect.Bottom); // sol dik
                e.Graphics.DrawLine(pen, rect.Right, rect.Top, rect.Right, rect.Bottom); // sağ dik
                e.Graphics.DrawLine(pen, rect.Left, rect.Bottom, rect.Right, rect.Bottom); // alt
            }

            // Yazı çizimi (TextRenderer ile)
            TextRenderer.DrawText(e.Graphics, text, labelFont, new Point(offset, 0), textColor);
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
            activemacrolbl.ForeColor = Color.FromArgb(188, 217, 232);
        }

        private void activemacrolbl_MouseLeave(object sender, EventArgs e)
        {
            activemacrolbl.ForeColor = Color.White;
            activemacrolbl.Font = new Font(activemacrolbl.Font, isModified ? FontStyle.Bold : FontStyle.Regular);
        }

        private void settingsBtn_Click(object sender, EventArgs e)
        {
            // open settings
            using (var settingsForm = new SettingsMenu(_hotkey, _settings, ToggleClicker))
            {
                if (settingsForm.ShowDialog() == DialogResult.OK)
                {

                    var (key, modifiers) = ConvertToKeySafe(_settings.Hotkey);
                    int fsModifiers = HotkeyService.ConvertModifiersToFsModifiers(modifiers);

                    _hotkey = new HotkeyService(this.Handle, key, ToggleClicker, fsModifiers);

                    ChangeLanguage((Language)_settings.LanguageIndex);
                    LoadLanguage();

                    startBtn.Text = Resources.start + " (" + _settings.Hotkey.ToString() + ")";
                    stopBtn.Text = Resources.stop + " (" + _settings.Hotkey.ToString() + ")";

                    isModified = false;
                    UpdateMacroLabel();
                    activemacrolbl.Font = new Font(activemacrolbl.Font, FontStyle.Regular);

                }
            }
        }

        public static (Keys key, Keys modifiers) ConvertToKeySafe(string hotkeyString)
        {
            try
            {
                string[] parts = hotkeyString.Split(new[] { " + " }, StringSplitOptions.RemoveEmptyEntries);
                Keys modifiers = Keys.None;
                Keys mainKey = Keys.None;

                foreach (string part in parts)
                {
                    string trimmed = part.Trim().ToLower();

                    switch (trimmed)
                    {
                        case "ctrl":
                        case "control":
                            modifiers |= Keys.Control;
                            break;
                        case "shift":
                            modifiers |= Keys.Shift;
                            break;
                        case "alt":
                            modifiers |= Keys.Alt;
                            break;
                        default:
                            if (Enum.TryParse(part, true, out Keys parsedKey))
                            {
                                mainKey = parsedKey;
                            }
                            else
                            {
                                throw new Exception($"Invalid Hotkey: {part}");
                            }
                            break;
                    }
                }

                return (mainKey, modifiers);
            }
            catch (Exception ex)
            {
                Console.WriteLine("Hotkey parse error: " + ex.Message);
                throw ex;
            }
        }

        private void infolbl_Click(object sender, EventArgs e)
        {
            System.Diagnostics.Process.Start("https://github.com/ilpenSE/autoclicker");
        }

        private void infolbl_MouseHover(object sender, EventArgs e)
        {
            infolbl.Font = new Font(infolbl.Font, FontStyle.Underline);
            infolbl.ForeColor = Color.FromArgb(170, 170, 170);
        }

        private void infolbl_MouseLeave(object sender, EventArgs e)
        {
            infolbl.Font = new Font(infolbl.Font, FontStyle.Regular);
            infolbl.ForeColor = Color.FromArgb(110, 110, 110);
        }

    }
}