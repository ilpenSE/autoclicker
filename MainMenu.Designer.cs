namespace AutoClicker
{
    partial class MainMenu
    {
        /// <summary>
        ///Gerekli tasarımcı değişkeni.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        ///Kullanılan tüm kaynakları temizleyin.
        /// </summary>
        ///<param name="disposing">yönetilen kaynaklar dispose edilmeliyse doğru; aksi halde yanlış.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer üretilen kod

        /// <summary>
        /// Tasarımcı desteği için gerekli metot - bu metodun 
        ///içeriğini kod düzenleyici ile değiştirmeyin.
        /// </summary>
        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainMenu));
            this.interval = new System.Windows.Forms.GroupBox();
            this.irv_millislbl = new System.Windows.Forms.Label();
            this.irvMillis = new Krypton.Toolkit.KryptonNumericUpDown();
            this.irvInfo = new System.Windows.Forms.Label();
            this.repeat = new System.Windows.Forms.GroupBox();
            this.repeatTimes = new Krypton.Toolkit.KryptonNumericUpDown();
            this.repeatTimesBtn = new Krypton.Toolkit.KryptonRadioButton();
            this.repeatForeverBtn = new Krypton.Toolkit.KryptonRadioButton();
            this.timeslbl = new System.Windows.Forms.Label();
            this.options = new System.Windows.Forms.GroupBox();
            this.cmodBtn = new Krypton.Toolkit.KryptonComboBox();
            this.cmodelbl = new System.Windows.Forms.Label();
            this.msButtonBtn = new Krypton.Toolkit.KryptonComboBox();
            this.clickTypeBtn = new Krypton.Toolkit.KryptonComboBox();
            this.clicktypelbl = new System.Windows.Forms.Label();
            this.msbtnlbl = new System.Windows.Forms.Label();
            this.position = new System.Windows.Forms.GroupBox();
            this.posY = new Krypton.Toolkit.KryptonNumericUpDown();
            this.posX = new Krypton.Toolkit.KryptonNumericUpDown();
            this.specPosBtn = new Krypton.Toolkit.KryptonRadioButton();
            this.currentPosBtn = new Krypton.Toolkit.KryptonRadioButton();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.activemacrolbl = new System.Windows.Forms.Label();
            this.infolbl = new System.Windows.Forms.Label();
            this.settingsBtn = new Krypton.Toolkit.KryptonButton();
            this.stopBtn = new Krypton.Toolkit.KryptonButton();
            this.startBtn = new Krypton.Toolkit.KryptonButton();
            this.holddur = new System.Windows.Forms.GroupBox();
            this.hd_millislbl = new System.Windows.Forms.Label();
            this.hdMillis = new Krypton.Toolkit.KryptonNumericUpDown();
            this.interval.SuspendLayout();
            this.repeat.SuspendLayout();
            this.options.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.cmodBtn)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.msButtonBtn)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.clickTypeBtn)).BeginInit();
            this.position.SuspendLayout();
            this.holddur.SuspendLayout();
            this.SuspendLayout();
            // 
            // interval
            // 
            this.interval.Controls.Add(this.irv_millislbl);
            this.interval.Controls.Add(this.irvMillis);
            this.interval.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F);
            this.interval.Location = new System.Drawing.Point(12, 72);
            this.interval.Name = "interval";
            this.interval.Size = new System.Drawing.Size(224, 75);
            this.interval.TabIndex = 0;
            this.interval.TabStop = false;
            this.interval.Text = "Interval";
            // 
            // irv_millislbl
            // 
            this.irv_millislbl.AutoSize = true;
            this.irv_millislbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F);
            this.irv_millislbl.Location = new System.Drawing.Point(149, 35);
            this.irv_millislbl.Name = "irv_millislbl";
            this.irv_millislbl.Size = new System.Drawing.Size(24, 15);
            this.irv_millislbl.TabIndex = 18;
            this.irv_millislbl.Text = "ms";
            // 
            // irvMillis
            // 
            this.irvMillis.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.irvMillis.Location = new System.Drawing.Point(26, 29);
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
            this.irvMillis.StateCommon.Border.Rounding = 5F;
            this.irvMillis.StateCommon.Border.Width = 1;
            this.irvMillis.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.irvMillis.TabIndex = 17;
            this.irvMillis.ThousandsSeparator = true;
            this.irvMillis.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // irvInfo
            // 
            this.irvInfo.AutoSize = true;
            this.irvInfo.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F);
            this.irvInfo.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(120)))), ((int)(((byte)(120)))));
            this.irvInfo.Location = new System.Drawing.Point(12, 52);
            this.irvInfo.Name = "irvInfo";
            this.irvInfo.Size = new System.Drawing.Size(176, 15);
            this.irvInfo.TabIndex = 19;
            this.irvInfo.Text = "1 seconds = 1000 milliseconds";
            // 
            // repeat
            // 
            this.repeat.Controls.Add(this.repeatTimes);
            this.repeat.Controls.Add(this.repeatTimesBtn);
            this.repeat.Controls.Add(this.repeatForeverBtn);
            this.repeat.Controls.Add(this.timeslbl);
            this.repeat.Location = new System.Drawing.Point(12, 251);
            this.repeat.Name = "repeat";
            this.repeat.Size = new System.Drawing.Size(224, 133);
            this.repeat.TabIndex = 1;
            this.repeat.TabStop = false;
            this.repeat.Text = "Repeat";
            // 
            // repeatTimes
            // 
            this.repeatTimes.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.repeatTimes.Location = new System.Drawing.Point(101, 31);
            this.repeatTimes.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.repeatTimes.Minimum = new decimal(new int[] {
            0,
            0,
            0,
            0});
            this.repeatTimes.Name = "repeatTimes";
            this.repeatTimes.Size = new System.Drawing.Size(75, 28);
            this.repeatTimes.StateActive.Border.Width = 2;
            this.repeatTimes.StateCommon.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.repeatTimes.StateCommon.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.repeatTimes.StateCommon.Border.Rounding = 5F;
            this.repeatTimes.StateCommon.Border.Width = 1;
            this.repeatTimes.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.repeatTimes.TabIndex = 18;
            this.repeatTimes.ThousandsSeparator = true;
            this.repeatTimes.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // repeatTimesBtn
            // 
            this.repeatTimesBtn.Location = new System.Drawing.Point(14, 33);
            this.repeatTimesBtn.Name = "repeatTimesBtn";
            this.repeatTimesBtn.Size = new System.Drawing.Size(61, 20);
            this.repeatTimesBtn.StateCommon.ShortText.Color1 = System.Drawing.Color.White;
            this.repeatTimesBtn.StateNormal.ShortText.Color1 = System.Drawing.Color.White;
            this.repeatTimesBtn.TabIndex = 13;
            this.repeatTimesBtn.Values.Text = "Repeat";
            // 
            // repeatForeverBtn
            // 
            this.repeatForeverBtn.Location = new System.Drawing.Point(14, 69);
            this.repeatForeverBtn.Name = "repeatForeverBtn";
            this.repeatForeverBtn.Size = new System.Drawing.Size(139, 20);
            this.repeatForeverBtn.StateCommon.ShortText.Color1 = System.Drawing.Color.White;
            this.repeatForeverBtn.TabIndex = 12;
            this.repeatForeverBtn.Values.Text = "Repeat Until Stopped";
            // 
            // timeslbl
            // 
            this.timeslbl.AutoSize = true;
            this.timeslbl.Location = new System.Drawing.Point(177, 39);
            this.timeslbl.Name = "timeslbl";
            this.timeslbl.Size = new System.Drawing.Size(31, 13);
            this.timeslbl.TabIndex = 8;
            this.timeslbl.Text = "times";
            // 
            // options
            // 
            this.options.Controls.Add(this.cmodBtn);
            this.options.Controls.Add(this.cmodelbl);
            this.options.Controls.Add(this.msButtonBtn);
            this.options.Controls.Add(this.clickTypeBtn);
            this.options.Controls.Add(this.clicktypelbl);
            this.options.Controls.Add(this.msbtnlbl);
            this.options.Location = new System.Drawing.Point(242, 251);
            this.options.Name = "options";
            this.options.Size = new System.Drawing.Size(238, 133);
            this.options.TabIndex = 1;
            this.options.TabStop = false;
            this.options.Text = "Options";
            // 
            // cmodBtn
            // 
            this.cmodBtn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmodBtn.DropDownWidth = 140;
            this.cmodBtn.Items.AddRange(new object[] {
            "Press",
            "Hold"});
            this.cmodBtn.Location = new System.Drawing.Point(124, 98);
            this.cmodBtn.Name = "cmodBtn";
            this.cmodBtn.Size = new System.Drawing.Size(109, 28);
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
            this.cmodBtn.TabIndex = 15;
            // 
            // cmodelbl
            // 
            this.cmodelbl.AutoSize = true;
            this.cmodelbl.Location = new System.Drawing.Point(9, 102);
            this.cmodelbl.Name = "cmodelbl";
            this.cmodelbl.Size = new System.Drawing.Size(54, 13);
            this.cmodelbl.TabIndex = 14;
            this.cmodelbl.Text = "Click Mod";
            // 
            // msButtonBtn
            // 
            this.msButtonBtn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.msButtonBtn.DropDownWidth = 140;
            this.msButtonBtn.Items.AddRange(new object[] {
            "Left",
            "Right",
            "Mid"});
            this.msButtonBtn.Location = new System.Drawing.Point(123, 24);
            this.msButtonBtn.Name = "msButtonBtn";
            this.msButtonBtn.Size = new System.Drawing.Size(109, 28);
            this.msButtonBtn.StateCommon.ComboBox.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
            this.msButtonBtn.StateCommon.ComboBox.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(70)))));
            this.msButtonBtn.StateCommon.ComboBox.Border.Rounding = 5F;
            this.msButtonBtn.StateCommon.ComboBox.Border.Width = 2;
            this.msButtonBtn.StateCommon.ComboBox.Content.Color1 = System.Drawing.Color.White;
            this.msButtonBtn.StateCommon.DropBack.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
            this.msButtonBtn.StateNormal.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(48)))));
            this.msButtonBtn.StateNormal.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(90)))), ((int)(((byte)(95)))));
            this.msButtonBtn.StateNormal.Item.Border.Rounding = 5F;
            this.msButtonBtn.StateNormal.Item.Border.Width = 2;
            this.msButtonBtn.StateTracking.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.msButtonBtn.StateTracking.Item.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.msButtonBtn.StateTracking.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.msButtonBtn.StateTracking.Item.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.msButtonBtn.StateTracking.Item.Border.Rounding = 5F;
            this.msButtonBtn.StateTracking.Item.Border.Width = 2;
            this.msButtonBtn.StateTracking.Item.Content.ShortText.Color1 = System.Drawing.Color.White;
            this.msButtonBtn.StateTracking.Item.Content.ShortText.Color2 = System.Drawing.Color.White;
            this.msButtonBtn.TabIndex = 13;
            // 
            // clickTypeBtn
            // 
            this.clickTypeBtn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.clickTypeBtn.DropDownWidth = 140;
            this.clickTypeBtn.Items.AddRange(new object[] {
            "Single",
            "Double",
            "Triple",
            "Quadruple"});
            this.clickTypeBtn.Location = new System.Drawing.Point(123, 61);
            this.clickTypeBtn.Name = "clickTypeBtn";
            this.clickTypeBtn.Size = new System.Drawing.Size(109, 28);
            this.clickTypeBtn.StateCommon.ComboBox.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(30)))), ((int)(((byte)(30)))), ((int)(((byte)(30)))));
            this.clickTypeBtn.StateCommon.ComboBox.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(70)))), ((int)(((byte)(70)))));
            this.clickTypeBtn.StateCommon.ComboBox.Border.Rounding = 5F;
            this.clickTypeBtn.StateCommon.ComboBox.Border.Width = 2;
            this.clickTypeBtn.StateCommon.ComboBox.Content.Color1 = System.Drawing.Color.White;
            this.clickTypeBtn.StateCommon.DropBack.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(180)))), ((int)(((byte)(180)))));
            this.clickTypeBtn.StateNormal.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(45)))), ((int)(((byte)(45)))), ((int)(((byte)(48)))));
            this.clickTypeBtn.StateNormal.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(90)))), ((int)(((byte)(95)))));
            this.clickTypeBtn.StateNormal.Item.Border.Rounding = 5F;
            this.clickTypeBtn.StateNormal.Item.Border.Width = 2;
            this.clickTypeBtn.StateTracking.Item.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.clickTypeBtn.StateTracking.Item.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(55)))), ((int)(((byte)(55)))), ((int)(((byte)(60)))));
            this.clickTypeBtn.StateTracking.Item.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.clickTypeBtn.StateTracking.Item.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(100)))), ((int)(((byte)(105)))));
            this.clickTypeBtn.StateTracking.Item.Border.Rounding = 5F;
            this.clickTypeBtn.StateTracking.Item.Border.Width = 2;
            this.clickTypeBtn.StateTracking.Item.Content.ShortText.Color1 = System.Drawing.Color.White;
            this.clickTypeBtn.StateTracking.Item.Content.ShortText.Color2 = System.Drawing.Color.White;
            this.clickTypeBtn.TabIndex = 12;
            // 
            // clicktypelbl
            // 
            this.clicktypelbl.AutoSize = true;
            this.clicktypelbl.Location = new System.Drawing.Point(8, 65);
            this.clicktypelbl.Name = "clicktypelbl";
            this.clicktypelbl.Size = new System.Drawing.Size(57, 13);
            this.clicktypelbl.TabIndex = 9;
            this.clicktypelbl.Text = "Click Type";
            // 
            // msbtnlbl
            // 
            this.msbtnlbl.AutoSize = true;
            this.msbtnlbl.Location = new System.Drawing.Point(8, 29);
            this.msbtnlbl.Name = "msbtnlbl";
            this.msbtnlbl.Size = new System.Drawing.Size(73, 13);
            this.msbtnlbl.TabIndex = 8;
            this.msbtnlbl.Text = "Mouse Button";
            // 
            // position
            // 
            this.position.Controls.Add(this.posY);
            this.position.Controls.Add(this.posX);
            this.position.Controls.Add(this.specPosBtn);
            this.position.Controls.Add(this.currentPosBtn);
            this.position.Controls.Add(this.label8);
            this.position.Controls.Add(this.label7);
            this.position.Location = new System.Drawing.Point(12, 151);
            this.position.Name = "position";
            this.position.Size = new System.Drawing.Size(468, 94);
            this.position.TabIndex = 2;
            this.position.TabStop = false;
            this.position.Text = "Position";
            // 
            // posY
            // 
            this.posY.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.posY.Location = new System.Drawing.Point(379, 36);
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
            this.posY.Size = new System.Drawing.Size(75, 28);
            this.posY.StateActive.Border.Width = 2;
            this.posY.StateCommon.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.posY.StateCommon.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.posY.StateCommon.Border.Rounding = 5F;
            this.posY.StateCommon.Border.Width = 1;
            this.posY.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.posY.TabIndex = 17;
            this.posY.ThousandsSeparator = true;
            this.posY.Value = new decimal(new int[] {
            0,
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
            this.posX.Location = new System.Drawing.Point(289, 36);
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
            this.posX.Size = new System.Drawing.Size(75, 28);
            this.posX.StateActive.Border.Width = 2;
            this.posX.StateCommon.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.posX.StateCommon.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.posX.StateCommon.Border.Rounding = 5F;
            this.posX.StateCommon.Border.Width = 1;
            this.posX.StateCommon.Content.Color1 = System.Drawing.Color.White;
            this.posX.TabIndex = 16;
            this.posX.ThousandsSeparator = true;
            this.posX.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // specPosBtn
            // 
            this.specPosBtn.Location = new System.Drawing.Point(164, 40);
            this.specPosBtn.Name = "specPosBtn";
            this.specPosBtn.Size = new System.Drawing.Size(122, 20);
            this.specPosBtn.StateCommon.ShortText.Color1 = System.Drawing.Color.White;
            this.specPosBtn.TabIndex = 15;
            this.specPosBtn.Values.Text = "Specified Location";
            // 
            // currentPosBtn
            // 
            this.currentPosBtn.Location = new System.Drawing.Point(16, 40);
            this.currentPosBtn.Name = "currentPosBtn";
            this.currentPosBtn.Size = new System.Drawing.Size(111, 20);
            this.currentPosBtn.StateCommon.ShortText.Color1 = System.Drawing.Color.White;
            this.currentPosBtn.TabIndex = 14;
            this.currentPosBtn.Values.Text = "Current Position";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(379, 20);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(14, 13);
            this.label8.TabIndex = 10;
            this.label8.Text = "Y";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(290, 20);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(14, 13);
            this.label7.TabIndex = 8;
            this.label7.Text = "X";
            // 
            // activemacrolbl
            // 
            this.activemacrolbl.AutoSize = true;
            this.activemacrolbl.Cursor = System.Windows.Forms.Cursors.Hand;
            this.activemacrolbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 12F);
            this.activemacrolbl.Location = new System.Drawing.Point(12, 21);
            this.activemacrolbl.Name = "activemacrolbl";
            this.activemacrolbl.Size = new System.Drawing.Size(269, 20);
            this.activemacrolbl.TabIndex = 9;
            this.activemacrolbl.Text = "Active Macro: #MCR (Tap to change)";
            this.activemacrolbl.Click += new System.EventHandler(this.activemacrolbl_Click);
            this.activemacrolbl.MouseEnter += new System.EventHandler(this.activemacrolbl_MouseEnter);
            this.activemacrolbl.MouseLeave += new System.EventHandler(this.activemacrolbl_MouseLeave);
            // 
            // infolbl
            // 
            this.infolbl.AutoSize = true;
            this.infolbl.Cursor = System.Windows.Forms.Cursors.Hand;
            this.infolbl.Font = new System.Drawing.Font("Microsoft YaHei", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.infolbl.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(110)))), ((int)(((byte)(110)))), ((int)(((byte)(110)))));
            this.infolbl.Location = new System.Drawing.Point(391, 442);
            this.infolbl.Name = "infolbl";
            this.infolbl.Size = new System.Drawing.Size(104, 16);
            this.infolbl.TabIndex = 10;
            this.infolbl.Text = "ilpeN © 2025, v1.3";
            this.infolbl.Click += new System.EventHandler(this.infolbl_Click);
            this.infolbl.MouseEnter += new System.EventHandler(this.infolbl_MouseHover);
            this.infolbl.MouseLeave += new System.EventHandler(this.infolbl_MouseLeave);
            // 
            // settingsBtn
            // 
            this.settingsBtn.Location = new System.Drawing.Point(329, 393);
            this.settingsBtn.Name = "settingsBtn";
            this.settingsBtn.Size = new System.Drawing.Size(152, 46);
            this.settingsBtn.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(130)))), ((int)(((byte)(180)))), ((int)(((byte)(255)))));
            this.settingsBtn.StateNormal.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(140)))), ((int)(((byte)(230)))));
            this.settingsBtn.StateNormal.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(120)))), ((int)(((byte)(200)))));
            this.settingsBtn.StateNormal.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(120)))), ((int)(((byte)(200)))));
            this.settingsBtn.StateNormal.Border.Rounding = 5F;
            this.settingsBtn.StateNormal.Border.Width = 2;
            this.settingsBtn.StatePressed.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(130)))), ((int)(((byte)(200)))));
            this.settingsBtn.StatePressed.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(100)))), ((int)(((byte)(160)))));
            this.settingsBtn.StatePressed.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(90)))), ((int)(((byte)(160)))));
            this.settingsBtn.StatePressed.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(50)))), ((int)(((byte)(90)))), ((int)(((byte)(160)))));
            this.settingsBtn.StatePressed.Border.Rounding = 5F;
            this.settingsBtn.StatePressed.Border.Width = 2;
            this.settingsBtn.StateTracking.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(160)))), ((int)(((byte)(200)))), ((int)(((byte)(255)))));
            this.settingsBtn.StateTracking.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(110)))), ((int)(((byte)(160)))), ((int)(((byte)(240)))));
            this.settingsBtn.StateTracking.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(140)))), ((int)(((byte)(230)))));
            this.settingsBtn.StateTracking.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(140)))), ((int)(((byte)(230)))));
            this.settingsBtn.StateTracking.Border.Rounding = 5F;
            this.settingsBtn.StateTracking.Border.Width = 2;
            this.settingsBtn.TabIndex = 14;
            this.settingsBtn.Values.DropDownArrowColor = System.Drawing.Color.Empty;
            this.settingsBtn.Values.Text = "SETTINGS";
            // 
            // stopBtn
            // 
            this.stopBtn.Enabled = false;
            this.stopBtn.Location = new System.Drawing.Point(171, 393);
            this.stopBtn.Name = "stopBtn";
            this.stopBtn.Size = new System.Drawing.Size(152, 46);
            this.stopBtn.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(140)))), ((int)(((byte)(140)))));
            this.stopBtn.StateNormal.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.stopBtn.StateNormal.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.stopBtn.StateNormal.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.stopBtn.StateNormal.Border.Rounding = 5F;
            this.stopBtn.StateNormal.Border.Width = 2;
            this.stopBtn.StatePressed.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(210)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.stopBtn.StatePressed.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(160)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.stopBtn.StatePressed.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.stopBtn.StatePressed.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.stopBtn.StatePressed.Border.Rounding = 5F;
            this.stopBtn.StatePressed.Border.Width = 2;
            this.stopBtn.StateTracking.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(160)))), ((int)(((byte)(160)))));
            this.stopBtn.StateTracking.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
            this.stopBtn.StateTracking.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(220)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.stopBtn.StateTracking.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(220)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.stopBtn.StateTracking.Border.Rounding = 5F;
            this.stopBtn.StateTracking.Border.Width = 2;
            this.stopBtn.TabIndex = 15;
            this.stopBtn.Values.DropDownArrowColor = System.Drawing.Color.Empty;
            this.stopBtn.Values.Text = "STOP";
            // 
            // startBtn
            // 
            this.startBtn.Location = new System.Drawing.Point(13, 393);
            this.startBtn.Name = "startBtn";
            this.startBtn.Size = new System.Drawing.Size(152, 46);
            this.startBtn.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(230)))), ((int)(((byte)(150)))));
            this.startBtn.StateNormal.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(200)))), ((int)(((byte)(90)))));
            this.startBtn.StateNormal.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(170)))), ((int)(((byte)(80)))));
            this.startBtn.StateNormal.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(170)))), ((int)(((byte)(80)))));
            this.startBtn.StateNormal.Border.Rounding = 5F;
            this.startBtn.StateNormal.Border.Width = 2;
            this.startBtn.StatePressed.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(110)))), ((int)(((byte)(190)))), ((int)(((byte)(110)))));
            this.startBtn.StatePressed.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(160)))), ((int)(((byte)(70)))));
            this.startBtn.StatePressed.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(140)))), ((int)(((byte)(60)))));
            this.startBtn.StatePressed.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(140)))), ((int)(((byte)(60)))));
            this.startBtn.StatePressed.Border.Rounding = 5F;
            this.startBtn.StatePressed.Border.Width = 2;
            this.startBtn.StateTracking.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(250)))), ((int)(((byte)(180)))));
            this.startBtn.StateTracking.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(220)))), ((int)(((byte)(120)))));
            this.startBtn.StateTracking.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(200)))), ((int)(((byte)(100)))));
            this.startBtn.StateTracking.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(200)))), ((int)(((byte)(100)))));
            this.startBtn.StateTracking.Border.Rounding = 5F;
            this.startBtn.StateTracking.Border.Width = 2;
            this.startBtn.TabIndex = 16;
            this.startBtn.Values.DropDownArrowColor = System.Drawing.Color.Empty;
            this.startBtn.Values.Text = "START";
            // 
            // holddur
            // 
            this.holddur.Controls.Add(this.hd_millislbl);
            this.holddur.Controls.Add(this.hdMillis);
            this.holddur.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F);
            this.holddur.Location = new System.Drawing.Point(242, 71);
            this.holddur.Name = "holddur";
            this.holddur.Size = new System.Drawing.Size(238, 75);
            this.holddur.TabIndex = 20;
            this.holddur.TabStop = false;
            this.holddur.Text = "Hold Duration";
            // 
            // hd_millislbl
            // 
            this.hd_millislbl.AutoSize = true;
            this.hd_millislbl.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F);
            this.hd_millislbl.Location = new System.Drawing.Point(154, 35);
            this.hd_millislbl.Name = "hd_millislbl";
            this.hd_millislbl.Size = new System.Drawing.Size(24, 15);
            this.hd_millislbl.TabIndex = 18;
            this.hd_millislbl.Text = "ms";
            // 
            // hdMillis
            // 
            this.hdMillis.Increment = new decimal(new int[] {
            1,
            0,
            0,
            0});
            this.hdMillis.Location = new System.Drawing.Point(29, 29);
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
            this.hdMillis.TabIndex = 17;
            this.hdMillis.ThousandsSeparator = true;
            this.hdMillis.Value = new decimal(new int[] {
            0,
            0,
            0,
            0});
            // 
            // MainMenu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(20)))), ((int)(((byte)(20)))));
            this.ClientSize = new System.Drawing.Size(492, 459);
            this.Controls.Add(this.irvInfo);
            this.Controls.Add(this.holddur);
            this.Controls.Add(this.startBtn);
            this.Controls.Add(this.stopBtn);
            this.Controls.Add(this.settingsBtn);
            this.Controls.Add(this.infolbl);
            this.Controls.Add(this.activemacrolbl);
            this.Controls.Add(this.position);
            this.Controls.Add(this.options);
            this.Controls.Add(this.repeat);
            this.Controls.Add(this.interval);
            this.ForeColor = System.Drawing.Color.White;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "MainMenu";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Auto Clicker";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainMenu_FormClosing);
            this.Load += new System.EventHandler(this.MainMenu_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.MainMenu_KeyDown);
            this.interval.ResumeLayout(false);
            this.interval.PerformLayout();
            this.repeat.ResumeLayout(false);
            this.repeat.PerformLayout();
            this.options.ResumeLayout(false);
            this.options.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.cmodBtn)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.msButtonBtn)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.clickTypeBtn)).EndInit();
            this.position.ResumeLayout(false);
            this.position.PerformLayout();
            this.holddur.ResumeLayout(false);
            this.holddur.PerformLayout();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox interval;
        private System.Windows.Forms.GroupBox repeat;
        private System.Windows.Forms.GroupBox options;
        private System.Windows.Forms.GroupBox position;
        private System.Windows.Forms.Label msbtnlbl;
        private System.Windows.Forms.Label clicktypelbl;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.Label timeslbl;
        private System.Windows.Forms.Label activemacrolbl;
        private System.Windows.Forms.Label infolbl;
        private Krypton.Toolkit.KryptonButton settingsBtn;
        private Krypton.Toolkit.KryptonButton stopBtn;
        private Krypton.Toolkit.KryptonButton startBtn;
        private Krypton.Toolkit.KryptonComboBox clickTypeBtn;
        private Krypton.Toolkit.KryptonComboBox msButtonBtn;
        private Krypton.Toolkit.KryptonRadioButton repeatForeverBtn;
        private Krypton.Toolkit.KryptonRadioButton repeatTimesBtn;
        private Krypton.Toolkit.KryptonRadioButton currentPosBtn;
        private Krypton.Toolkit.KryptonRadioButton specPosBtn;
        private Krypton.Toolkit.KryptonNumericUpDown posX;
        private Krypton.Toolkit.KryptonNumericUpDown posY;
        private System.Windows.Forms.Label irv_millislbl;
        private Krypton.Toolkit.KryptonNumericUpDown irvMillis;
        private System.Windows.Forms.Label irvInfo;
        private Krypton.Toolkit.KryptonComboBox cmodBtn;
        private System.Windows.Forms.Label cmodelbl;
        private System.Windows.Forms.GroupBox holddur;
        private System.Windows.Forms.Label hd_millislbl;
        private Krypton.Toolkit.KryptonNumericUpDown hdMillis;
        private Krypton.Toolkit.KryptonNumericUpDown repeatTimes;
    }
}

