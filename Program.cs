using System;
using System.IO;
using System.Windows.Forms;

namespace AutoClicker
{
    public static class Program
    {
        public static readonly string appDataDir = Path.Combine(
                Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData),
                "AutoClicker"
            ); public static string settingsPath = Path.Combine(appDataDir, "settings.json");
        public static string macrosPath = Path.Combine(appDataDir, "macros.json");

        [STAThread]
        static void Main()
        {
            try
            {
                // 📁 Dizin yoksa oluştur
                if (!Directory.Exists(appDataDir))
                    Directory.CreateDirectory(appDataDir);

                Console.WriteLine(settingsPath);
                Console.WriteLine(macrosPath);

                // ✅ Dosya yoksa oluştur
                if (!File.Exists(settingsPath))
                    File.WriteAllText(settingsPath, "{\r\n  \"Hotkey\": \"F6\",\r\n  \"LanguageIndex\": 0,\r\n  \"FirstRun\": true,\r\n  \"ActiveMacro\": \"DEFAULT\",\r\n  \"Version\": \"v1.3\"\r\n}");

                if (!File.Exists(macrosPath))
                    File.WriteAllText(macrosPath, "{\r\n  \"DEFAULT\": {\r\n    \"Name\": \"DEFAULT\",\r\n    \"Description\": \"The defaults.\",\r\n    \"MouseButton\": 0,\r\n    \"ClickType\": 0,\r\n    \"Position\": {\r\n      \"X\": 0,\r\n      \"Y\": 0,\r\n      \"CurrentPosition\": true\r\n    },\r\n    \"Interval\": 0,\r\n    \"RepeatTimes\": {\r\n      \"Count\": 1,\r\n      \"RepeatForever\": true\r\n    },\r\n    \"ClickMode\": 0,\r\n    \"HoldDuration\": 1\r\n  }\r\n}");

                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.Run(new MainMenu());
            }
            catch (Exception ex)
            {
                File.WriteAllText("error_log.txt", ex.ToString());
                MessageBox.Show("An error occured! Saved to error_log.txt.\nError Message: " + ex.Message, "Auto Clicker", MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }
    }
}
