using AutoClicker.Properties;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Localization;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Globalization;
using System.Windows.Documents;
using System.Windows.Forms;

namespace AutoClicker
{
    public partial class SettingsMenu : BaseForm
    {
        private SettingsModel _settings;
        private HotkeyService _hotkey;

        public SettingsMenu(HotkeyService hotkey, SettingsModel settings)
        {
            InitializeComponent();
            this._settings = settings;
            this._hotkey = hotkey;
            Select();
            Focus();
        }

        private void SettingsMenu_Load(object sender, EventArgs e)
        {
            LoadLanguage();
            LoadValues();
            this.Font = RegularFont;
            titlelbl.Font = FontLoader.GetBold(18f);
            UpdateInfoLabel();

            saveBtn.StateCommon.Content.ShortText.Font = MainMenu.ButtonFont;
            discardBtn.StateCommon.Content.ShortText.Font = MainMenu.ButtonFont;

            langBtn.MouseEnter += (s, ev) =>
                {
                    langBtn.StateActive.ComboBox.Border.Color1 = MainMenu.focusBorderClr;
                };

            langBtn.MouseLeave += (s, ev) =>
                {
                    langBtn.StateActive.ComboBox.Border.Color1 =  MainMenu.normalBorderClr;
                };

            langBtn.GotFocus += (s, ev) =>
                {
                    langBtn.StateActive.ComboBox.Border.Color1 = MainMenu.focusBorderClr;
                };

            langBtn.LostFocus += (s, ev) =>
            {
                langBtn.StateActive.ComboBox.Border.Color1 = MainMenu.normalBorderClr;
            };
        }


        private void UpdateInfoLabel()
        {
            infolbl.Text = $"ilpeN © 2025, v{MainMenu.VERSION_STRING}";
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

        private void LoadValues()
        {
            langBtn.SelectedIndex = _settings.LanguageIndex;
            htkbox.Text = _settings.Hotkey.ToString();
        }

        bool isWaitingForHotkey = false;

        public override void ReloadLanguage()
        {
            LoadLanguage();
        }

        private void LoadLanguage()
        {
            this.Text = Resources.settings_title;
            langlbl.Text = Resources.opt_lang;
            htklbl.Text = Resources.htk;
            titlelbl.Text = Resources.settings_title;
            saveBtn.Text = Resources.save;
            discardBtn.Text = Resources.cancel;
        }

        private void saveBtn_Click(object sender, EventArgs e)
        {
            // 1. Dil kaydet
            _settings.LanguageIndex = langBtn.SelectedIndex;

            // 2. Hotkey string'ini kaydet (örnek: "Ctrl + F7")
            string hotkeyText = htkbox.Text;

            if (string.IsNullOrWhiteSpace(hotkeyText))
            {
                MessageBox.Show("Hotkey cannot be empty.");
                return;
            }
            _hotkey.Dispose();

            _settings.Hotkey = hotkeyText;

            // 3. Ayarları kaydet
            SettingsManager.Save(_settings);

            // 4. Formu kapat
            this.DialogResult = DialogResult.OK;
            this.Close();
        }

        Font RegularFont = FontLoader.GetRegular(9f);

        private void discardBtn_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.Cancel;
            this.Close();
        }

        private void ToggleHotkeyWaiting(object sender, EventArgs e)
        {
            isWaitingForHotkey = !isWaitingForHotkey;

            this.KeyPreview = !isWaitingForHotkey;

            htkbox.StateCommon.Border.Color1 = isWaitingForHotkey ? MainMenu.focusBorderClr : MainMenu.normalBorderClr;

            Console.WriteLine(isWaitingForHotkey);
        }

        private void htkbox_KeyDown(object sender, KeyEventArgs e)
        {
            if (isWaitingForHotkey)
            {
                uint modifiers = 0;
                List<string> parts = new List<string>();

                if (e.Control)
                {
                    modifiers |= 0x0002; // MOD_CONTROL
                    parts.Add("Ctrl");
                }
                if (e.Shift)
                {
                    modifiers |= 0x0004; // MOD_SHIFT
                    parts.Add("Shift");
                }
                if (e.Alt)
                {
                    modifiers |= 0x0001; // MOD_ALT
                    parts.Add("Alt");
                }

                bool isModifierKey =
                    e.KeyCode == Keys.ControlKey ||
                    e.KeyCode == Keys.ShiftKey ||
                    e.KeyCode == Keys.Menu;

                if (!isModifierKey)
                {
                    parts.Add(e.KeyCode.ToString());

                    string hotkeyString = string.Join(" + ", parts);

                    htkbox.Text = hotkeyString;

                    // Olayın başka yerlere geçmesini engelle
                    e.Handled = true;
                    e.SuppressKeyPress = true;
                }
                else
                {
                    // Sadece modifier tuşuna basıldıysa textbox'u temizle veya beklet
                    htkbox.Text = string.Join(" + ", parts);
                }
            }
        }

        private void htkbox_MouseHover(object sender, EventArgs e)
        {
            htkbox.StateCommon.Border.Color1 = MainMenu.focusBorderClr;
        }

        private void htkbox_MouseLeave(object sender, EventArgs e)
        {
            if (!isWaitingForHotkey)
                htkbox.StateCommon.Border.Color1 = MainMenu.normalBorderClr;
        }

        private void htkbox_MouseEnter(object sender, EventArgs e)
        {
            if (!isWaitingForHotkey)
                htkbox.StateCommon.Border.Color1 = MainMenu.focusBorderClr;
        }
    }
}
