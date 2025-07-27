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
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);

                CheckAndCreateFilesAndDirs();
                var updateAvailable = CheckUpdate().GetAwaiter().GetResult();

                if (updateAvailable)
                {
                    var dlg = new UpdateDialog();
                    dlg.ShowDialog();
                }

                Application.Run(new MainMenu());

            }
            catch (Exception ex)
            {
                File.WriteAllText("error_log.txt", ex.ToString());
                MessageBox.Show(Resources.err_occured + ex.Message, "Auto Clicker", MessageBoxButtons.OK, MessageBoxIcon.Error);
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

    }
}
