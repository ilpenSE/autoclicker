using AutoClicker.Properties;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Localization;
using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Diagnostics;
using System.Drawing;
using System.Globalization;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace AutoClicker
{
    public partial class UpdateDialog : BaseForm
    {
        bool IsTherePreReleaseUpdate = false;
        string PreReleaseURL = "";

        public UpdateDialog(string preReleaseURL)
        {
            IsTherePreReleaseUpdate = !string.IsNullOrWhiteSpace(preReleaseURL);
            PreReleaseURL = preReleaseURL;

            InitializeComponent();
            buttonYes.Click += ButtonYes_Click;
            buttonNo.Click += ButtonNo_Click;
            LoadLanguage();
        }

        public override void ReloadLanguage()
        {
            LoadLanguage();
        }

        public void LoadLanguage()
        {
            labelMessage.Text = IsTherePreReleaseUpdate ? Resources.preupdate_available_txt : Resources.update_available_txt;
            this.Text = Resources.update_available_title;
            buttonYes.Text = Resources.download;
            buttonNo.Text = MainMenu.CapitalizeFirstLetter(Resources.cancel);
        }

        private void ButtonYes_Click(object sender, EventArgs e)
        {
            // İndir butonuna basıldığında varsayılan tarayıcıda GitHub release sayfasını aç
            Process.Start(new ProcessStartInfo
            {
                FileName = IsTherePreReleaseUpdate ? PreReleaseURL : "https://github.com/ilpenSE/autoclicker/releases/latest",
                UseShellExecute = true
            });

            this.DialogResult = DialogResult.Yes;
            this.Close();
        }

        private void ButtonNo_Click(object sender, EventArgs e)
        {
            this.DialogResult = DialogResult.No;
            this.Close();
        }

        private void UpdateDialog_Load(object sender, EventArgs e)
        {
            this.Select();
        }
    }
}
