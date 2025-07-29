using System;
using System.Windows.Forms;

namespace AutoClicker
{
    partial class MacroCreateForm
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
        private System.Windows.Forms.Label lblName;
        private System.Windows.Forms.Label lblDesc;
        private System.Windows.Forms.Label lblX;
        private System.Windows.Forms.Label lblY;

        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MacroCreateForm));
            this.lblName = new System.Windows.Forms.Label();
            this.lblDesc = new System.Windows.Forms.Label();
            this.lblX = new System.Windows.Forms.Label();
            this.lblY = new System.Windows.Forms.Label();
            this.repeattimeslbl = new System.Windows.Forms.Label();
            this.intervallbl = new System.Windows.Forms.Label();
            this.irvInfo = new System.Windows.Forms.Label();
            this.millislbl = new System.Windows.Forms.Label();
            this.irvMillis = new Krypton.Toolkit.KryptonNumericUpDown();
            this.cmodBtn = new Krypton.Toolkit.KryptonComboBox();
            this.lblMod = new System.Windows.Forms.Label();
            this.cmbMouseButton = new Krypton.Toolkit.KryptonComboBox();
            this.cmbClickType = new Krypton.Toolkit.KryptonComboBox();
            this.lblClick = new System.Windows.Forms.Label();
            this.lblMouse = new System.Windows.Forms.Label();
            this.repeatTimes = new Krypton.Toolkit.KryptonNumericUpDown();
            this.posX = new Krypton.Toolkit.KryptonNumericUpDown();
            this.posY = new Krypton.Toolkit.KryptonNumericUpDown();
            this.repeatForeverBtn = new Krypton.Toolkit.KryptonCheckBox();
            this.hd_millislbl = new System.Windows.Forms.Label();
            this.hdMillis = new Krypton.Toolkit.KryptonNumericUpDown();
            this.holddurlbl = new System.Windows.Forms.Label();
            this.chkUseCurrentPos = new Krypton.Toolkit.KryptonCheckBox();
            this.btnSave = new Krypton.Toolkit.KryptonButton();
            this.txtName = new Krypton.Toolkit.KryptonTextBox();
            this.txtDescription = new Krypton.Toolkit.KryptonTextBox();
            ((System.ComponentModel.ISupportInitialize)(this.cmodBtn)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cmbMouseButton)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.cmbClickType)).BeginInit();
            this.SuspendLayout();
            // 
            // lblName
            // 
            this.lblName.Location = new System.Drawing.Point(12, 16);
            this.lblName.Name = "lblName";
            this.lblName.Size = new System.Drawing.Size(100, 23);
            this.lblName.TabIndex = 1;
            this.lblName.Text = "Name:";
            // 
            // lblDesc
            // 
            this.lblDesc.Location = new System.Drawing.Point(12, 48);
            this.lblDesc.Name = "lblDesc";
            this.lblDesc.Size = new System.Drawing.Size(100, 23);
            this.lblDesc.TabIndex = 3;
            this.lblDesc.Text = "Description:";
            // 
            // lblX
            // 
            this.lblX.Location = new System.Drawing.Point(12, 376);
            this.lblX.Name = "lblX";
            this.lblX.Size = new System.Drawing.Size(100, 23);
            this.lblX.TabIndex = 10;
            this.lblX.Text = "Position X:";
            // 
            // lblY
            // 
            this.lblY.Location = new System.Drawing.Point(12, 412);
            this.lblY.Name = "lblY";
            this.lblY.Size = new System.Drawing.Size(100, 23);
            this.lblY.TabIndex = 12;
            this.lblY.Text = "Position Y:";
            // 
            // repeattimeslbl
            // 
            this.repeattimeslbl.Location = new System.Drawing.Point(12, 310);
            this.repeattimeslbl.Name = "repeattimeslbl";
            this.repeattimeslbl.Size = new System.Drawing.Size(100, 23);
            this.repeattimeslbl.TabIndex = 15;
            this.repeattimeslbl.Text = "Repeat:";
            // 
            // intervallbl
            // 
            this.intervallbl.Location = new System.Drawing.Point(12, 195);
            this.intervallbl.Name = "intervallbl";
            this.intervallbl.Size = new System.Drawing.Size(100, 23);
            this.intervallbl.TabIndex = 17;
            this.intervallbl.Text = "Interval:";
            // 
            // irvInfo
            // 
            this.irvInfo.AutoSize = true;
            this.irvInfo.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F);
            this.irvInfo.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(140)))), ((int)(((byte)(140)))), ((int)(((byte)(140)))));
            this.irvInfo.Location = new System.Drawing.Point(151, 276);
            this.irvInfo.Name = "irvInfo";
            this.irvInfo.Size = new System.Drawing.Size(176, 15);
            this.irvInfo.TabIndex = 22;
            this.irvInfo.Text = "1 seconds = 1000 milliseconds";
            // 
            // millislbl
            // 
            this.millislbl.AutoSize = true;
            this.millislbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F);
            this.millislbl.Location = new System.Drawing.Point(275, 199);
            this.millislbl.Name = "millislbl";
            this.millislbl.Size = new System.Drawing.Size(76, 15);
            this.millislbl.TabIndex = 21;
            this.millislbl.Text = "milliseconds";
            // 
            // irvMillis
            // 
            this.irvMillis.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.irvMillis.Location = new System.Drawing.Point(151, 191);
            this.irvMillis.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.irvMillis.Minimum = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.irvMillis.Name = "irvMillis";
            this.irvMillis.Size = new System.Drawing.Size(120, 28);
            this.irvMillis.StateActive.Border.Width = 2;
            this.irvMillis.StateCommon.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.irvMillis.StateCommon.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.irvMillis.StateCommon.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.irvMillis.StateCommon.Border.Rounding = 5F;
            this.irvMillis.StateCommon.Border.Width = 1;
            this.irvMillis.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.irvMillis.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.irvMillis.StateNormal.Content.Color1 = System.Drawing.Color.White;
            this.irvMillis.TabIndex = 20;
            this.irvMillis.ThousandsSeparator = true;
            this.irvMillis.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // cmodBtn
            // 
            this.cmodBtn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmodBtn.DropDownWidth = 140;
            this.cmodBtn.Items.AddRange(new object[] {
            "Press",
            "Hold"});
            this.cmodBtn.Location = new System.Drawing.Point(149, 152);
            this.cmodBtn.Name = "cmodBtn";
            this.cmodBtn.Size = new System.Drawing.Size(202, 28);
            this.cmodBtn.StateCommon.ComboBox.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
            this.cmodBtn.StateCommon.ComboBox.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(70)))));
            this.cmodBtn.StateCommon.ComboBox.Border.Rounding = 5F;
            this.cmodBtn.StateCommon.ComboBox.Border.Width = 2;
            this.cmodBtn.StateCommon.ComboBox.Content.Color1 = System.Drawing.Color.White;
            this.cmodBtn.StateCommon.DropBack.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
            this.cmodBtn.StateNormal.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(48)))));
            this.cmodBtn.StateNormal.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(90)))), ((int)(((byte)(95)))));
            this.cmodBtn.StateNormal.Item.Border.Rounding = 5F;
            this.cmodBtn.StateNormal.Item.Border.Width = 2;
            this.cmodBtn.StateTracking.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.cmodBtn.StateTracking.Item.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.cmodBtn.StateTracking.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.cmodBtn.StateTracking.Item.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.cmodBtn.StateTracking.Item.Border.Rounding = 5F;
            this.cmodBtn.StateTracking.Item.Border.Width = 2;
            this.cmodBtn.StateTracking.Item.Content.ShortText.Color1 = System.Drawing.Color.White;
            this.cmodBtn.StateTracking.Item.Content.ShortText.Color2 = System.Drawing.Color.White;
            this.cmodBtn.TabIndex = 28;
            // 
            // lblMod
            // 
            this.lblMod.AutoSize = true;
            this.lblMod.Location = new System.Drawing.Point(13, 156);
            this.lblMod.Name = "lblMod";
            this.lblMod.Size = new System.Drawing.Size(54, 13);
            this.lblMod.TabIndex = 27;
            this.lblMod.Text = "Click Mod";
            // 
            // cmbMouseButton
            // 
            this.cmbMouseButton.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbMouseButton.DropDownWidth = 140;
            this.cmbMouseButton.Items.AddRange(new object[] {
            "Left",
            "Right",
            "Mid"});
            this.cmbMouseButton.Location = new System.Drawing.Point(148, 78);
            this.cmbMouseButton.Name = "cmbMouseButton";
            this.cmbMouseButton.Size = new System.Drawing.Size(203, 28);
            this.cmbMouseButton.StateCommon.ComboBox.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
            this.cmbMouseButton.StateCommon.ComboBox.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(70)))));
            this.cmbMouseButton.StateCommon.ComboBox.Border.Rounding = 5F;
            this.cmbMouseButton.StateCommon.ComboBox.Border.Width = 2;
            this.cmbMouseButton.StateCommon.ComboBox.Content.Color1 = System.Drawing.Color.White;
            this.cmbMouseButton.StateCommon.DropBack.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
            this.cmbMouseButton.StateNormal.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(48)))));
            this.cmbMouseButton.StateNormal.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(90)))), ((int)(((byte)(95)))));
            this.cmbMouseButton.StateNormal.Item.Border.Rounding = 5F;
            this.cmbMouseButton.StateNormal.Item.Border.Width = 2;
            this.cmbMouseButton.StateTracking.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.cmbMouseButton.StateTracking.Item.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.cmbMouseButton.StateTracking.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.cmbMouseButton.StateTracking.Item.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.cmbMouseButton.StateTracking.Item.Border.Rounding = 5F;
            this.cmbMouseButton.StateTracking.Item.Border.Width = 2;
            this.cmbMouseButton.StateTracking.Item.Content.ShortText.Color1 = System.Drawing.Color.White;
            this.cmbMouseButton.StateTracking.Item.Content.ShortText.Color2 = System.Drawing.Color.White;
            this.cmbMouseButton.TabIndex = 26;
            // 
            // cmbClickType
            // 
            this.cmbClickType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbClickType.DropDownWidth = 140;
            this.cmbClickType.Items.AddRange(new object[] {
            "Single",
            "Double",
            "Triple",
            "Quadruple",
            "Hold"});
            this.cmbClickType.Location = new System.Drawing.Point(148, 115);
            this.cmbClickType.Name = "cmbClickType";
            this.cmbClickType.Size = new System.Drawing.Size(203, 28);
            this.cmbClickType.StateCommon.ComboBox.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
            this.cmbClickType.StateCommon.ComboBox.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(70)))));
            this.cmbClickType.StateCommon.ComboBox.Border.Rounding = 5F;
            this.cmbClickType.StateCommon.ComboBox.Border.Width = 2;
            this.cmbClickType.StateCommon.ComboBox.Content.Color1 = System.Drawing.Color.White;
            this.cmbClickType.StateCommon.DropBack.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
            this.cmbClickType.StateNormal.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(48)))));
            this.cmbClickType.StateNormal.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(90)))), ((int)(((byte)(95)))));
            this.cmbClickType.StateNormal.Item.Border.Rounding = 5F;
            this.cmbClickType.StateNormal.Item.Border.Width = 2;
            this.cmbClickType.StateTracking.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.cmbClickType.StateTracking.Item.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.cmbClickType.StateTracking.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.cmbClickType.StateTracking.Item.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.cmbClickType.StateTracking.Item.Border.Rounding = 5F;
            this.cmbClickType.StateTracking.Item.Border.Width = 2;
            this.cmbClickType.StateTracking.Item.Content.ShortText.Color1 = System.Drawing.Color.White;
            this.cmbClickType.StateTracking.Item.Content.ShortText.Color2 = System.Drawing.Color.White;
            this.cmbClickType.TabIndex = 25;
            // 
            // lblClick
            // 
            this.lblClick.AutoSize = true;
            this.lblClick.Location = new System.Drawing.Point(12, 119);
            this.lblClick.Name = "lblClick";
            this.lblClick.Size = new System.Drawing.Size(57, 13);
            this.lblClick.TabIndex = 24;
            this.lblClick.Text = "Click Type";
            // 
            // lblMouse
            // 
            this.lblMouse.AutoSize = true;
            this.lblMouse.Location = new System.Drawing.Point(12, 83);
            this.lblMouse.Name = "lblMouse";
            this.lblMouse.Size = new System.Drawing.Size(73, 13);
            this.lblMouse.TabIndex = 23;
            this.lblMouse.Text = "Mouse Button";
            // 
            // repeatTimes
            // 
            this.repeatTimes.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.repeatTimes.Location = new System.Drawing.Point(151, 305);
            this.repeatTimes.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.repeatTimes.Minimum = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.repeatTimes.Name = "repeatTimes";
            this.repeatTimes.Size = new System.Drawing.Size(120, 28);
            this.repeatTimes.StateActive.Border.Width = 2;
            this.repeatTimes.StateCommon.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.repeatTimes.StateCommon.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.repeatTimes.StateCommon.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.repeatTimes.StateCommon.Border.Rounding = 5F;
            this.repeatTimes.StateCommon.Border.Width = 1;
            this.repeatTimes.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.repeatTimes.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.repeatTimes.StateNormal.Content.Color1 = System.Drawing.Color.White;
            this.repeatTimes.TabIndex = 29;
            this.repeatTimes.ThousandsSeparator = true;
            this.repeatTimes.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // posX
            // 
            this.posX.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.posX.Location = new System.Drawing.Point(151, 371);
            this.posX.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.posX.Minimum = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.posX.Name = "posX";
            this.posX.Size = new System.Drawing.Size(120, 28);
            this.posX.StateActive.Border.Width = 2;
            this.posX.StateCommon.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.posX.StateCommon.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.posX.StateCommon.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.posX.StateCommon.Border.Rounding = 5F;
            this.posX.StateCommon.Border.Width = 1;
            this.posX.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.posX.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.posX.StateNormal.Content.Color1 = System.Drawing.Color.White;
            this.posX.TabIndex = 30;
            this.posX.ThousandsSeparator = true;
            this.posX.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // posY
            // 
            this.posY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.posY.Location = new System.Drawing.Point(151, 408);
            this.posY.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.posY.Minimum = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.posY.Name = "posY";
            this.posY.Size = new System.Drawing.Size(120, 28);
            this.posY.StateActive.Border.Width = 2;
            this.posY.StateCommon.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.posY.StateCommon.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.posY.StateCommon.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.posY.StateCommon.Border.Rounding = 5F;
            this.posY.StateCommon.Border.Width = 1;
            this.posY.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.posY.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.posY.StateNormal.Content.Color1 = System.Drawing.Color.White;
            this.posY.TabIndex = 31;
            this.posY.ThousandsSeparator = true;
            this.posY.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // repeatForeverBtn
            // 
            this.repeatForeverBtn.Checked = true;
            this.repeatForeverBtn.CheckState = System.Windows.Forms.CheckState.Checked;
            this.repeatForeverBtn.Location = new System.Drawing.Point(151, 341);
            this.repeatForeverBtn.Name = "repeatForeverBtn";
            this.repeatForeverBtn.Size = new System.Drawing.Size(140, 20);
            this.repeatForeverBtn.StateNormal.ShortText.Color1 = System.Drawing.Color.White;
            this.repeatForeverBtn.TabIndex = 32;
            this.repeatForeverBtn.Values.Text = "Repeat Until Stopped";
            // 
            // hd_millislbl
            // 
            this.hd_millislbl.AutoSize = true;
            this.hd_millislbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F);
            this.hd_millislbl.Location = new System.Drawing.Point(277, 245);
            this.hd_millislbl.Name = "hd_millislbl";
            this.hd_millislbl.Size = new System.Drawing.Size(24, 15);
            this.hd_millislbl.TabIndex = 34;
            this.hd_millislbl.Text = "ms";
            // 
            // hdMillis
            // 
            this.hdMillis.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.hdMillis.Location = new System.Drawing.Point(151, 238);
            this.hdMillis.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.hdMillis.Minimum = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.hdMillis.Name = "hdMillis";
            this.hdMillis.Size = new System.Drawing.Size(120, 28);
            this.hdMillis.StateActive.Border.Width = 2;
            this.hdMillis.StateCommon.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.hdMillis.StateCommon.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.hdMillis.StateCommon.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.hdMillis.StateCommon.Border.Rounding = 5F;
            this.hdMillis.StateCommon.Border.Width = 1;
            this.hdMillis.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.hdMillis.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.hdMillis.StateNormal.Content.Color1 = System.Drawing.Color.White;
            this.hdMillis.TabIndex = 33;
            this.hdMillis.ThousandsSeparator = true;
            this.hdMillis.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // holddurlbl
            // 
            this.holddurlbl.Location = new System.Drawing.Point(13, 241);
            this.holddurlbl.Name = "holddurlbl";
            this.holddurlbl.Size = new System.Drawing.Size(100, 23);
            this.holddurlbl.TabIndex = 35;
            this.holddurlbl.Text = "Hold Duration:";
            // 
            // chkUseCurrentPos
            // 
            this.chkUseCurrentPos.Checked = true;
            this.chkUseCurrentPos.CheckState = System.Windows.Forms.CheckState.Checked;
            this.chkUseCurrentPos.Location = new System.Drawing.Point(151, 446);
            this.chkUseCurrentPos.Name = "chkUseCurrentPos";
            this.chkUseCurrentPos.Size = new System.Drawing.Size(135, 20);
            this.chkUseCurrentPos.StateNormal.ShortText.Color1 = System.Drawing.Color.White;
            this.chkUseCurrentPos.TabIndex = 36;
            this.chkUseCurrentPos.Values.Text = "Use Current Position";
            // 
            // btnSave
            // 
            this.btnSave.Location = new System.Drawing.Point(239, 480);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(123, 46);
            this.btnSave.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(230)))), ((int)(((byte)(150)))));
            this.btnSave.StateNormal.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(200)))), ((int)(((byte)(90)))));
            this.btnSave.StateNormal.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(170)))), ((int)(((byte)(80)))));
            this.btnSave.StateNormal.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(170)))), ((int)(((byte)(80)))));
            this.btnSave.StateNormal.Border.Rounding = 5F;
            this.btnSave.StateNormal.Border.Width = 2;
            this.btnSave.StatePressed.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(110)))), ((int)(((byte)(190)))), ((int)(((byte)(110)))));
            this.btnSave.StatePressed.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(160)))), ((int)(((byte)(70)))));
            this.btnSave.StatePressed.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(140)))), ((int)(((byte)(60)))));
            this.btnSave.StatePressed.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(140)))), ((int)(((byte)(60)))));
            this.btnSave.StatePressed.Border.Rounding = 5F;
            this.btnSave.StatePressed.Border.Width = 2;
            this.btnSave.StateTracking.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(250)))), ((int)(((byte)(180)))));
            this.btnSave.StateTracking.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(220)))), ((int)(((byte)(120)))));
            this.btnSave.StateTracking.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(200)))), ((int)(((byte)(100)))));
            this.btnSave.StateTracking.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(200)))), ((int)(((byte)(100)))));
            this.btnSave.StateTracking.Border.Rounding = 5F;
            this.btnSave.StateTracking.Border.Width = 2;
            this.btnSave.TabIndex = 37;
            this.btnSave.Values.DropDownArrowColor = System.Drawing.Color.Empty;
            this.btnSave.Values.Text = "Create";
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // txtName
            // 
            this.txtName.Location = new System.Drawing.Point(148, 12);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(203, 27);
            this.txtName.StateCommon.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.txtName.StateCommon.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
            this.txtName.StateCommon.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
            this.txtName.StateCommon.Border.Rounding = 3F;
            this.txtName.StateCommon.Border.Width = 2;
            this.txtName.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.txtName.TabIndex = 38;
            this.txtName.Tag = "";
            // 
            // txtDescription
            // 
            this.txtDescription.Location = new System.Drawing.Point(148, 45);
            this.txtDescription.Name = "txtDescription";
            this.txtDescription.Size = new System.Drawing.Size(203, 27);
            this.txtDescription.StateCommon.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.txtDescription.StateCommon.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
            this.txtDescription.StateCommon.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
            this.txtDescription.StateCommon.Border.Rounding = 3F;
            this.txtDescription.StateCommon.Border.Width = 2;
            this.txtDescription.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.txtDescription.TabIndex = 39;
            // 
            // MacroCreateForm
            // 
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(20)))), ((int)(((byte)(20)))));
            this.ClientSize = new System.Drawing.Size(370, 535);
            this.Controls.Add(this.txtDescription);
            this.Controls.Add(this.txtName);
            this.Controls.Add(this.btnSave);
            this.Controls.Add(this.chkUseCurrentPos);
            this.Controls.Add(this.holddurlbl);
            this.Controls.Add(this.hd_millislbl);
            this.Controls.Add(this.hdMillis);
            this.Controls.Add(this.repeatForeverBtn);
            this.Controls.Add(this.posY);
            this.Controls.Add(this.posX);
            this.Controls.Add(this.repeatTimes);
            this.Controls.Add(this.cmodBtn);
            this.Controls.Add(this.lblMod);
            this.Controls.Add(this.cmbMouseButton);
            this.Controls.Add(this.cmbClickType);
            this.Controls.Add(this.lblClick);
            this.Controls.Add(this.lblMouse);
            this.Controls.Add(this.irvInfo);
            this.Controls.Add(this.millislbl);
            this.Controls.Add(this.irvMillis);
            this.Controls.Add(this.intervallbl);
            this.Controls.Add(this.repeattimeslbl);
            this.Controls.Add(this.lblName);
            this.Controls.Add(this.lblDesc);
            this.Controls.Add(this.lblX);
            this.Controls.Add(this.lblY);
            this.ForeColor = System.Drawing.Color.White;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MacroCreateForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Create New Macro";
            this.Load += new System.EventHandler(this.MacroCreateForm_Load);
            this.Controls.SetChildIndex(this.lblY, 0);
            this.Controls.SetChildIndex(this.lblX, 0);
            this.Controls.SetChildIndex(this.lblDesc, 0);
            this.Controls.SetChildIndex(this.lblName, 0);
            this.Controls.SetChildIndex(this.repeattimeslbl, 0);
            this.Controls.SetChildIndex(this.intervallbl, 0);
            this.Controls.SetChildIndex(this.irvMillis, 0);
            this.Controls.SetChildIndex(this.millislbl, 0);
            this.Controls.SetChildIndex(this.irvInfo, 0);
            this.Controls.SetChildIndex(this.lblMouse, 0);
            this.Controls.SetChildIndex(this.lblClick, 0);
            this.Controls.SetChildIndex(this.cmbClickType, 0);
            this.Controls.SetChildIndex(this.cmbMouseButton, 0);
            this.Controls.SetChildIndex(this.lblMod, 0);
            this.Controls.SetChildIndex(this.cmodBtn, 0);
            this.Controls.SetChildIndex(this.repeatTimes, 0);
            this.Controls.SetChildIndex(this.posX, 0);
            this.Controls.SetChildIndex(this.posY, 0);
            this.Controls.SetChildIndex(this.repeatForeverBtn, 0);
            this.Controls.SetChildIndex(this.hdMillis, 0);
            this.Controls.SetChildIndex(this.hd_millislbl, 0);
            this.Controls.SetChildIndex(this.holddurlbl, 0);
            this.Controls.SetChildIndex(this.chkUseCurrentPos, 0);
            this.Controls.SetChildIndex(this.btnSave, 0);
            this.Controls.SetChildIndex(this.txtName, 0);
            this.Controls.SetChildIndex(this.txtDescription, 0);
            ((System.ComponentModel.ISupportInitialize)(this.cmodBtn)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cmbMouseButton)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.cmbClickType)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }


        #endregion
        private Label repeattimeslbl;
        private Label intervallbl;
        private Label irvInfo;
        private Label millislbl;
        private Krypton.Toolkit.KryptonNumericUpDown irvMillis;
        private Krypton.Toolkit.KryptonComboBox cmodBtn;
        private Label lblMod;
        private Krypton.Toolkit.KryptonComboBox cmbMouseButton;
        private Krypton.Toolkit.KryptonComboBox cmbClickType;
        private Label lblClick;
        private Label lblMouse;
        private Krypton.Toolkit.KryptonNumericUpDown repeatTimes;
        private Krypton.Toolkit.KryptonNumericUpDown posX;
        private Krypton.Toolkit.KryptonNumericUpDown posY;
        private Krypton.Toolkit.KryptonCheckBox repeatForeverBtn;
        private Label hd_millislbl;
        private Krypton.Toolkit.KryptonNumericUpDown hdMillis;
        private Label holddurlbl;
        private Krypton.Toolkit.KryptonCheckBox chkUseCurrentPos;
        private Krypton.Toolkit.KryptonButton btnSave;
        private Krypton.Toolkit.KryptonTextBox txtName;
        private Krypton.Toolkit.KryptonTextBox txtDescription;
    }
}