using System;
using System.Collections.Generic;
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
            Application.EnableVisualStyles();
            Application.SetCompatibleTextRenderingDefault(false);
            Application.ApplicationExit += new EventHandler((sender, e) => {
                Properties.Settings.Default.Save();
            });
            AppDomain.CurrentDomain.ProcessExit += new EventHandler((sender, e) => { Properties.Settings.Default.Save(); });
            Application.Run(new MainMenu());
        }
    }
}
