using AutoClicker.Properties;
using Krypton.Toolkit;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Localization;
using Microsoft.VisualBasic;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Drawing2D;
using System.Globalization;
using System.Linq;
using System.Runtime;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AutoClicker
{
    public partial class MainMenu : BaseForm
    {
        private ClickerEngine _clicker;
        private HotkeyService _hotkey;
        private SettingsModel _settings;
        private Dictionary<string, MacroModel> _Macros = new Dictionary<string, MacroModel>();

        private string _activeMacroName = "DEFAULT";
        private bool isModified = false;
        public static readonly Dictionary<string, Language> LanguageMap = new Dictionary<string, Language>
        {
            { "en", Language.ENGLISH },
            { "tr", Language.TURKISH },
            { "de", Language.GERMAN },
            { "fr", Language.FRENCH },
            { "it", Language.ITALIAN }
        };

        public static readonly Dictionary<Language, string> LanguageReverseMap =
    LanguageMap.ToDictionary(kvp => kvp.Value, kvp => kvp.Key);

        private bool isClicking = false;
        private bool isWaitingForHotkey = false;
        private System.Windows.Forms.Timer ClickTimer = new System.Windows.Forms.Timer();
        private int repeats = 0;


        // UPDATE THIS WHEN YOU UPDATE THE PROGRAM
        public static readonly string VERSION_STRING = "1.3.3";

        public MainMenu()
        {
            InitializeComponent();
            
            this.KeyPreview = true;
            this.Load += MainMenu_Load_Heavy;
            this.Shown += MainMenu_Shown;
        }

        public override void ReloadLanguage()
        {
            LoadLanguage();
        }

        private async void MainMenu_Load_Heavy(object sender, EventArgs e)
        {
            // Run heavy IO works parallel
            _settings = await Task.Run(() => SettingsManager.Load()) ?? new SettingsModel();

            _Macros = await Task.Run(() => MacroManager.Macros) ?? new Dictionary<string, MacroModel>();

            // Makro control and default fallback
            if (!_Macros.ContainsKey(_settings.ActiveMacro))
            {
                _activeMacroName = "DEFAULT";
                _settings.ActiveMacro = "DEFAULT";
                SettingsManager.Save(_settings);
            }
            else
            {
                _activeMacroName = _settings.ActiveMacro;
            }

            // load Macro UI (in UI thread)
            await Task.Delay(100); // a small wait
            LoadMacroFormettings(_activeMacroName);
            UpdateMacroLabel();

            // Hotkey setup (needs UI handle)
            TryRegisterHotkey();

            // set the language if it's first run
            if (_settings.FirstRun)
            {
                ApplyDefaultLanguageBySystem();
                _settings.FirstRun = false;
            }

            // timer tick event
            ClickTimer.Tick += TimerTick;

            ValidateLanguageIndex();
            ChangeLanguage(_settings.LanguageIndex);
            LoadLanguage();
            RegisterChangeListeners();

            // save the settings
            await Task.Run(() => SettingsManager.Save(_settings));

        }

        // Hotkey registering
        private void TryRegisterHotkey()
        {
            try
            {
                var (key, modifiers) = ConvertToKeySafe(_settings.Hotkey);
                int fsModifiers = HotkeyService.ConvertModifiersToFsModifiers(modifiers);
                _hotkey = new HotkeyService(this.Handle, key, ToggleClicker, fsModifiers);
            }
            catch
            {
                // fallback hotkey
                _hotkey = new HotkeyService(this.Handle, SettingsManager.DefaultKey, ToggleClicker);
                _settings.Hotkey = SettingsManager.DefaultKey.ToString();
                SettingsManager.Save(_settings);

                MessageBox.Show(
                    Resources.wrn_invalidhtk,
                    Resources.wrn_title,
                    MessageBoxButtons.OK,
                    MessageBoxIcon.Warning
                );
            }
        }

        // Sistem diline göre ilk açılışta dil seçimi
        private void ApplyDefaultLanguageBySystem()
        {
            string defLang = CultureInfo.CurrentUICulture.Name.Substring(0, 2);
            Language lang = LanguageMap.ContainsKey(defLang)
                ? LanguageMap[defLang]
                : SettingsManager.DefaultLanguage;

            ChangeLanguage(lang);
        }

        private void MainMenu_Shown(object sender, EventArgs e)
        {
            RegisterNumericFocusEvents();
            RegisterDropdownFocusEvents();
            SetDisabledStatesOfButtons();

            Console.WriteLine($"[{DateTime.Now.ToString("HH:mm:ss:fff-dd/MM/yyyy")}] [LOAD] Loaded.");
        }

        // Dil indexi bozuksa düzelt
        private void ValidateLanguageIndex()
        {
            if (_settings.LanguageIndex > (LanguageMap.Count - 1) || _settings.LanguageIndex < 0)
            {
                _settings.LanguageIndex = (int)SettingsManager.DefaultLanguage;
                SettingsManager.Save(_settings);
            }
        }

        private void RegisterChangeListeners()
        {
            this.msButtonBtn.SelectedIndexChanged += MarkAsUnsaved;
            this.clickTypeBtn.SelectedIndexChanged += MarkAsUnsaved;
            this.cmodBtn.SelectedIndexChanged += MarkAsUnsaved;
            this.posY.ValueChanged += MarkAsUnsaved;
            this.posX.ValueChanged += MarkAsUnsaved;
            this.hdMillis.ValueChanged += MarkAsUnsaved;
            this.specPosBtn.CheckedChanged += MarkAsUnsaved;
            this.currentPosBtn.CheckedChanged += MarkAsUnsaved;
            this.irvMillis.ValueChanged += MarkAsUnsaved;
            this.repeatTimes.ValueChanged += MarkAsUnsaved;
            this.repeatForeverBtn.CheckedChanged += MarkAsUnsaved;
            this.repeatTimesBtn.CheckedChanged += MarkAsUnsaved;
        }

        private void MarkAsUnsaved(object sender, EventArgs e)
        {
            if (!isModified)
            {
                activemacrolbl.Font = ActiveMacroLabelFontBold;
                activemacrolbl.Text = Resources.activemacro_unsaved.Replace("{_activeMacroName}", _activeMacroName);
                isModified = true;
            }
        }

        public static readonly Font ActiveMacroLabelFontBold = FontLoader.GetBold(12f);
        public static readonly Font ActiveMacroLabelFont = FontLoader.GetRegular(12f);

        private void UpdateMacroLabel()
        {
            activemacrolbl.Text = Resources.activemacro.Replace("{_activeMacroName}", (_activeMacroName == "DEFAULT" ? Resources.defaultmacro : _activeMacroName));
            activemacrolbl.Font = ActiveMacroLabelFont;
        }

        private void LoadMacroFormettings(string macroName)
        {
            try
            {
                if (_Macros == null || !_Macros.TryGetValue(macroName, out var macro)) return;

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
                string newName = Interaction.InputBox(Resources.ask_enternewmacro, "Auto Clicker");
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

        public static string GetLanguageCode(Language lang)
        {
            if (LanguageReverseMap.TryGetValue(lang, out var code))
                return code;

            return LanguageReverseMap[SettingsManager.DefaultLanguage];
        }
        public static string GetLanguageCode(int langIndex)
        {
            if (LanguageReverseMap.TryGetValue(GetLanguageByIndex(langIndex), out var code))
                return code;

            return LanguageReverseMap[SettingsManager.DefaultLanguage];
        }
        private void ChangeLanguage(int langIndex)
        {
            // index → Language enum
            Language lang = (Language)langIndex;
            ChangeLanguage(lang);
        }

        private void ChangeLanguage(Language lang)
        {
            string cultureCode = GetLanguageCode(lang);
            var newCulture = new CultureInfo(cultureCode);

            // Global kültürü değiştir
            CultureInfo.CurrentUICulture = newCulture;
            CultureInfo.CurrentCulture = newCulture;

            // Resources'un kültürünü zorla
            Resources.Culture = newCulture;

            // Tüm açık formları yenile
            foreach (Form openForm in Application.OpenForms)
            {
                if (openForm is BaseForm baseForm)
                    baseForm.ReloadLanguage();
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
        public static readonly Font GroupBoxFont = FontLoader.GetBold(10f);

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
            startBtn.Click += startBtn_Click;

            stopBtn.StateCommon.Content.ShortText.Font = ButtonFont;
            stopBtn.Click += stopBtn_Click;

            settingsBtn.StateCommon.Content.ShortText.Font = ButtonFont;
            settingsBtn.Click += settingsBtn_Click;
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
            Font labelFont = GroupBoxFont;
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
            using (var settingsForm = new SettingsMenu(_hotkey, _settings))
            {
                if (settingsForm.ShowDialog() == DialogResult.OK)
                {

                    var (key, modifiers) = ConvertToKeySafe(_settings.Hotkey);
                    int fsModifiers = HotkeyService.ConvertModifiersToFsModifiers(modifiers);

                    _hotkey = new HotkeyService(this.Handle, key, ToggleClicker, fsModifiers);

                    ChangeLanguage(_settings.LanguageIndex);
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