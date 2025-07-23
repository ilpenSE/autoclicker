namespace AutoClicker
{
    partial class SettingsMenu
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(SettingsMenu));
            this.langlbl = new System.Windows.Forms.Label();
            this.htklbl = new System.Windows.Forms.Label();
            this.titlelbl = new System.Windows.Forms.Label();
            this.langBtn = new Krypton.Toolkit.KryptonComboBox();
            this.saveBtn = new Krypton.Toolkit.KryptonButton();
            this.discardBtn = new Krypton.Toolkit.KryptonButton();
            this.htkbox = new Krypton.Toolkit.KryptonTextBox();
            ((System.ComponentModel.ISupportInitialize)(this.langBtn)).BeginInit();
            this.SuspendLayout();
            // 
            // langlbl
            // 
            this.langlbl.AutoSize = true;
            this.langlbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.langlbl.Location = new System.Drawing.Point(24, 62);
            this.langlbl.Name = "langlbl";
            this.langlbl.Size = new System.Drawing.Size(72, 17);
            this.langlbl.TabIndex = 15;
            this.langlbl.Text = "Language";
            // 
            // htklbl
            // 
            this.htklbl.AutoSize = true;
            this.htklbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.htklbl.Location = new System.Drawing.Point(24, 107);
            this.htklbl.Name = "htklbl";
            this.htklbl.Size = new System.Drawing.Size(52, 17);
            this.htklbl.TabIndex = 17;
            this.htklbl.Text = "Hotkey";
            // 
            // titlelbl
            // 
            this.titlelbl.AutoSize = true;
            this.titlelbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 16F);
            this.titlelbl.Location = new System.Drawing.Point(23, 21);
            this.titlelbl.Name = "titlelbl";
            this.titlelbl.Size = new System.Drawing.Size(91, 26);
            this.titlelbl.TabIndex = 19;
            this.titlelbl.Text = "Settings";
            // 
            // langBtn
            // 
            this.langBtn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.langBtn.DropDownWidth = 140;
            this.langBtn.Items.AddRange(new object[] {
            "English",
            "Türkçe",
            "Deutsch",
            "Français"});
            this.langBtn.Location = new System.Drawing.Point(148, 62);
            this.langBtn.Name = "langBtn";
            this.langBtn.Size = new System.Drawing.Size(132, 28);
            this.langBtn.StateActive.ComboBox.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(65)))));
            this.langBtn.StateActive.ComboBox.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(110)))), ((int)(((byte)(110)))), ((int)(((byte)(115)))));
            this.langBtn.StateActive.ComboBox.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(110)))), ((int)(((byte)(110)))), ((int)(((byte)(115)))));
            this.langBtn.StateActive.ComboBox.Border.Rounding = 5F;
            this.langBtn.StateActive.ComboBox.Border.Width = 2;
            this.langBtn.StateCommon.ComboBox.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
            this.langBtn.StateCommon.ComboBox.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(70)))));
            this.langBtn.StateCommon.ComboBox.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(70)))));
            this.langBtn.StateCommon.ComboBox.Border.Rounding = 5F;
            this.langBtn.StateCommon.ComboBox.Border.Width = 2;
            this.langBtn.StateCommon.ComboBox.Content.Color1 = System.Drawing.Color.White;
            this.langBtn.StateCommon.DropBack.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
            this.langBtn.StateCommon.DropBack.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
            this.langBtn.StateDisabled.ComboBox.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(20)))), ((int)(((byte)(20)))));
            this.langBtn.StateDisabled.ComboBox.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.langBtn.StateDisabled.ComboBox.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.langBtn.StateDisabled.ComboBox.Border.Rounding = 5F;
            this.langBtn.StateDisabled.ComboBox.Border.Width = 2;
            this.langBtn.StateDisabled.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(20)))), ((int)(((byte)(20)))));
            this.langBtn.StateDisabled.Item.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(20)))), ((int)(((byte)(20)))));
            this.langBtn.StateDisabled.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.langBtn.StateDisabled.Item.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.langBtn.StateDisabled.Item.Border.Rounding = 5F;
            this.langBtn.StateDisabled.Item.Border.Width = 2;
            this.langBtn.StateNormal.ComboBox.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(48)))));
            this.langBtn.StateNormal.ComboBox.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(90)))), ((int)(((byte)(95)))));
            this.langBtn.StateNormal.ComboBox.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(90)))), ((int)(((byte)(95)))));
            this.langBtn.StateNormal.ComboBox.Border.Rounding = 5F;
            this.langBtn.StateNormal.ComboBox.Border.Width = 2;
            this.langBtn.StateNormal.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(48)))));
            this.langBtn.StateNormal.Item.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(48)))));
            this.langBtn.StateNormal.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(90)))), ((int)(((byte)(95)))));
            this.langBtn.StateNormal.Item.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(90)))), ((int)(((byte)(95)))));
            this.langBtn.StateNormal.Item.Border.Rounding = 5F;
            this.langBtn.StateNormal.Item.Border.Width = 2;
            this.langBtn.StateTracking.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.langBtn.StateTracking.Item.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.langBtn.StateTracking.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.langBtn.StateTracking.Item.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.langBtn.StateTracking.Item.Border.Rounding = 5F;
            this.langBtn.StateTracking.Item.Border.Width = 2;
            this.langBtn.StateTracking.Item.Content.LongText.Color1 = System.Drawing.Color.White;
            this.langBtn.StateTracking.Item.Content.LongText.Color2 = System.Drawing.Color.White;
            this.langBtn.StateTracking.Item.Content.ShortText.Color1 = System.Drawing.Color.White;
            this.langBtn.StateTracking.Item.Content.ShortText.Color2 = System.Drawing.Color.White;
            this.langBtn.TabIndex = 22;
            // 
            // saveBtn
            // 
            this.saveBtn.Location = new System.Drawing.Point(23, 146);
            this.saveBtn.Name = "saveBtn";
            this.saveBtn.Size = new System.Drawing.Size(120, 46);
            this.saveBtn.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(230)))), ((int)(((byte)(150)))));
            this.saveBtn.StateNormal.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(200)))), ((int)(((byte)(90)))));
            this.saveBtn.StateNormal.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(170)))), ((int)(((byte)(80)))));
            this.saveBtn.StateNormal.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(170)))), ((int)(((byte)(80)))));
            this.saveBtn.StateNormal.Border.Rounding = 5F;
            this.saveBtn.StateNormal.Border.Width = 2;
            this.saveBtn.StatePressed.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(110)))), ((int)(((byte)(190)))), ((int)(((byte)(110)))));
            this.saveBtn.StatePressed.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(160)))), ((int)(((byte)(70)))));
            this.saveBtn.StatePressed.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(140)))), ((int)(((byte)(60)))));
            this.saveBtn.StatePressed.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(140)))), ((int)(((byte)(60)))));
            this.saveBtn.StatePressed.Border.Rounding = 5F;
            this.saveBtn.StatePressed.Border.Width = 2;
            this.saveBtn.StateTracking.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(250)))), ((int)(((byte)(180)))));
            this.saveBtn.StateTracking.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(220)))), ((int)(((byte)(120)))));
            this.saveBtn.StateTracking.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(200)))), ((int)(((byte)(100)))));
            this.saveBtn.StateTracking.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(200)))), ((int)(((byte)(100)))));
            this.saveBtn.StateTracking.Border.Rounding = 5F;
            this.saveBtn.StateTracking.Border.Width = 2;
            this.saveBtn.TabIndex = 24;
            this.saveBtn.Values.DropDownArrowColor = System.Drawing.Color.Empty;
            this.saveBtn.Values.Text = "SAVE";
            this.saveBtn.Click += new System.EventHandler(this.saveBtn_Click);
            // 
            // discardBtn
            // 
            this.discardBtn.Location = new System.Drawing.Point(155, 146);
            this.discardBtn.Name = "discardBtn";
            this.discardBtn.Size = new System.Drawing.Size(125, 46);
            this.discardBtn.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(140)))), ((int)(((byte)(140)))));
            this.discardBtn.StateNormal.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.discardBtn.StateNormal.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.discardBtn.StateNormal.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.discardBtn.StateNormal.Border.Rounding = 5F;
            this.discardBtn.StateNormal.Border.Width = 2;
            this.discardBtn.StatePressed.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(210)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.discardBtn.StatePressed.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(160)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.discardBtn.StatePressed.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.discardBtn.StatePressed.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.discardBtn.StatePressed.Border.Rounding = 5F;
            this.discardBtn.StatePressed.Border.Width = 2;
            this.discardBtn.StateTracking.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(160)))), ((int)(((byte)(160)))));
            this.discardBtn.StateTracking.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
            this.discardBtn.StateTracking.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(220)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.discardBtn.StateTracking.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(220)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.discardBtn.StateTracking.Border.Rounding = 5F;
            this.discardBtn.StateTracking.Border.Width = 2;
            this.discardBtn.TabIndex = 23;
            this.discardBtn.Values.DropDownArrowColor = System.Drawing.Color.Empty;
            this.discardBtn.Values.Text = "DISACRD";
            this.discardBtn.Click += new System.EventHandler(this.discardBtn_Click);
            // 
            // htkbox
            // 
            this.htkbox.Location = new System.Drawing.Point(148, 107);
            this.htkbox.Name = "htkbox";
            this.htkbox.Size = new System.Drawing.Size(132, 27);
            this.htkbox.StateCommon.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.htkbox.StateCommon.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
            this.htkbox.StateCommon.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
            this.htkbox.StateCommon.Border.Rounding = 3F;
            this.htkbox.StateCommon.Border.Width = 2;
            this.htkbox.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.htkbox.TabIndex = 25;
            this.htkbox.Enter += new System.EventHandler(this.ToggleHotkeyWaiting);
            this.htkbox.KeyDown += new System.Windows.Forms.KeyEventHandler(this.htkbox_KeyDown);
            this.htkbox.Leave += new System.EventHandler(this.ToggleHotkeyWaiting);
            this.htkbox.MouseEnter += new System.EventHandler(this.htkbox_MouseEnter);
            this.htkbox.MouseLeave += new System.EventHandler(this.htkbox_MouseLeave);
            // 
            // SettingsMenu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(20)))), ((int)(((byte)(20)))));
            this.ClientSize = new System.Drawing.Size(292, 212);
            this.Controls.Add(this.htkbox);
            this.Controls.Add(this.saveBtn);
            this.Controls.Add(this.discardBtn);
            this.Controls.Add(this.langBtn);
            this.Controls.Add(this.titlelbl);
            this.Controls.Add(this.htklbl);
            this.Controls.Add(this.langlbl);
            this.ForeColor = System.Drawing.Color.White;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Name = "SettingsMenu";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Settings";
            this.Load += new System.EventHandler(this.SettingsMenu_Load);
            ((System.ComponentModel.ISupportInitialize)(this.langBtn)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion
        private System.Windows.Forms.Label langlbl;
        private System.Windows.Forms.Label htklbl;
        private System.Windows.Forms.Label titlelbl;
        private Krypton.Toolkit.KryptonComboBox langBtn;
        private Krypton.Toolkit.KryptonButton saveBtn;
        private Krypton.Toolkit.KryptonButton discardBtn;
        private Krypton.Toolkit.KryptonTextBox htkbox;
    }
}