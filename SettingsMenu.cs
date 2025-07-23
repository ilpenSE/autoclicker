using AutoClicker.Properties;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Localization;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Globalization;
using System.IO.Ports;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AutoClicker
{
    public partial class SettingsMenu : Form
    {
        private SettingsModel _settings;
        private HotkeyService _hotkey;
        private Action toggleClicker;

        public SettingsMenu(HotkeyService hotkey, SettingsModel settings, Action onKeyPressed)
        {
            InitializeComponent();
            this._settings = settings;
            this._hotkey = hotkey;
            this.toggleClicker = onKeyPressed;
            Select();
            Focus();
            Text = Resources.settings_title;
        }

        private void SettingsMenu_Load(object sender, EventArgs e)
        {
            LoadLanguage();
            LoadValues();
            this.Font = FontLoader.GetRegular(9f);

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

        private void LoadValues()
        {
            langBtn.SelectedIndex = _settings.LanguageIndex;
            htkbox.Text = _settings.Hotkey.ToString();
        }

        bool isWaitingForHotkey = false;

        private void LoadLanguage()
        {
            var localizer = GetLocalizer();

            langlbl.Text = Resources.opt_lang;
            htklbl.Text = Resources.htk;
            titlelbl.Text = Resources.settings_title;
            saveBtn.Text = Resources.save;
            discardBtn.Text = Resources.discard;
        }

        private IStringLocalizer<Resources> GetLocalizer()
        {
            // this returns localizer which has localized texts
            var services = new ServiceCollection();
            services.AddLogging();
            services.AddLocalization(options => options.ResourcesPath = "Properties");

            var provider = services.BuildServiceProvider();

            // set the culture
            CultureInfo.CurrentUICulture = new CultureInfo(MainMenu.GetLanguageCode(GetLanguage()));

            // return localizer
            return provider.GetRequiredService<IStringLocalizer<Resources>>();
        }

        private Language GetLanguage()
        {
            return MainMenu.GetLanguageByIndex(_settings.LanguageIndex);
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
