using AutoClicker.Properties;
using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.Globalization;
using System.IO;
using System.Net.Http;
using System.Runtime;
using System.Runtime.CompilerServices;
using System.Text.Json;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AutoClicker
{
    public static class Program
    {
        public static readonly string appDataDir = Path.Combine(
                Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData),
                "AutoClicker"
            );
        public static string settingsPath = Path.Combine(appDataDir, "settings.json");
        public static string macrosPath = Path.Combine(appDataDir, "macros.json");

        [STAThread]
        static void Main()
        {
            Console.WriteLine($"[{DateTime.Now.ToString("HH:mm:ss:fff-dd/MM/yyyy")}] [LOAD] Loading...");
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                // Ayarları oku (örneğin settings.json’dan)
                var langIndex = LoadLanguageIndexFromSettings();

                // Culture ayarla
                var cultureCode = MainMenu.GetLanguageCode((Language)langIndex);
                var culture = new CultureInfo(cultureCode);

                CultureInfo.CurrentUICulture = culture;
                CultureInfo.CurrentCulture = culture;
                Resources.Culture = culture;

                CheckAndCreateFilesAndDirs();
                var updateAvailable = CheckUpdate().GetAwaiter().GetResult();
                var preReleaseUpdateURL = CheckPreReleaseUpdate().GetAwaiter().GetResult();

                Console.WriteLine(preReleaseUpdateURL);

                if (updateAvailable || preReleaseUpdateURL != "")
                {
                    var dlg = new UpdateDialog(preReleaseUpdateURL);
                    dlg.ShowDialog();

                    if (dlg.DialogResult == DialogResult.No)
                    {
                        Application.Run(new MainMenu());
                    }
                    return;
                }

                Application.Run(new MainMenu());

            }
            catch (Exception ex)
            {
                File.WriteAllText("error_log.txt", ex.ToString());
                MessageBox.Show(Resources.err_occured + ex.Message, "Auto Clicker", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
        private static int LoadLanguageIndexFromSettings()
        {
            if (!File.Exists(settingsPath))
                return (int)SettingsManager.DefaultLanguage;

            try
            {
                var json = File.ReadAllText(settingsPath);
                var settings = JsonSerializer.Deserialize<SettingsModel>(json);
                return settings?.LanguageIndex ?? (int)SettingsManager.DefaultLanguage;
            }
            catch
            {
                return 0;
            }
        }

        static void CheckAndCreateFilesAndDirs()
        {
            // 📁 Dizin yoksa oluştur
            if (!Directory.Exists(appDataDir))
                Directory.CreateDirectory(appDataDir);

            Console.WriteLine($"Settings.json path: {settingsPath}");
            Console.WriteLine($"Macros.json path: {macrosPath}");

            // ✅ Dosya yoksa oluştur
            if (!File.Exists(settingsPath))
                File.WriteAllText(settingsPath, "");

            if (!File.Exists(macrosPath))
                File.WriteAllText(macrosPath, "{}");
        }
        static async System.Threading.Tasks.Task<bool> CheckUpdate()
        {
            return await UpdateChecker.CheckUpdateAvailable();
        }
        static async System.Threading.Tasks.Task<string> CheckPreReleaseUpdate()
        {
            return await UpdateChecker.CheckPreReleaseAvailable();
        }

    }
}
