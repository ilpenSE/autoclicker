using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AutoClicker
{
    internal static class Program
    {
        /// <summary>
        /// Uygulamanın ana girdi noktası.
        /// </summary>
        [STAThread]
        static void Main()
        {
            try
            {
                Application.EnableVisualStyles();
                Application.SetCompatibleTextRenderingDefault(false);
                Application.ApplicationExit += new EventHandler((sender, e) => {
                    Properties.Settings.Default.Save();
                });
                AppDomain.CurrentDomain.ProcessExit += new EventHandler((sender, e) => { Properties.Settings.Default.Save(); });
                Application.Run(new MainMenu());
            }
            catch (Exception ex)
            {
                File.WriteAllText("error_log.txt", ex.ToString());
                MessageBox.Show("An error occured! Saved to error_log.txt.");
            }
        }
    }
}
