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
            this.millislbl = new System.Windows.Forms.Label();
            this.irvMillis = new System.Windows.Forms.NumericUpDown();
            this.seclbl = new System.Windows.Forms.Label();
            this.irvSecs = new System.Windows.Forms.NumericUpDown();
            this.minlbl = new System.Windows.Forms.Label();
            this.hrslbl = new System.Windows.Forms.Label();
            this.irvMins = new System.Windows.Forms.NumericUpDown();
            this.irvHrs = new System.Windows.Forms.NumericUpDown();
            this.repeat = new System.Windows.Forms.GroupBox();
            this.timeslbl = new System.Windows.Forms.Label();
            this.repeatTimes = new System.Windows.Forms.NumericUpDown();
            this.repeatForeverBtn = new System.Windows.Forms.RadioButton();
            this.repeatTimesBtn = new System.Windows.Forms.RadioButton();
            this.options = new System.Windows.Forms.GroupBox();
            this.langBtn = new System.Windows.Forms.ComboBox();
            this.langlbl = new System.Windows.Forms.Label();
            this.clickTypeBtn = new System.Windows.Forms.ComboBox();
            this.msButtonBtn = new System.Windows.Forms.ComboBox();
            this.clicktypelbl = new System.Windows.Forms.Label();
            this.msbtnlbl = new System.Windows.Forms.Label();
            this.position = new System.Windows.Forms.GroupBox();
            this.label8 = new System.Windows.Forms.Label();
            this.label7 = new System.Windows.Forms.Label();
            this.posY = new System.Windows.Forms.NumericUpDown();
            this.posX = new System.Windows.Forms.NumericUpDown();
            this.specPosBtn = new System.Windows.Forms.RadioButton();
            this.currentPosBtn = new System.Windows.Forms.RadioButton();
            this.startBtn = new System.Windows.Forms.Button();
            this.stopBtn = new System.Windows.Forms.Button();
            this.setHtkBtn = new System.Windows.Forms.Button();
            this.activemacrolbl = new System.Windows.Forms.Label();
            this.label1 = new System.Windows.Forms.Label();
            this.interval.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.irvMillis)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvSecs)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvMins)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvHrs)).BeginInit();
            this.repeat.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.repeatTimes)).BeginInit();
            this.options.SuspendLayout();
            this.position.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.posY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.posX)).BeginInit();
            this.SuspendLayout();
            // 
            // interval
            // 
            this.interval.Controls.Add(this.millislbl);
            this.interval.Controls.Add(this.irvMillis);
            this.interval.Controls.Add(this.seclbl);
            this.interval.Controls.Add(this.irvSecs);
            this.interval.Controls.Add(this.minlbl);
            this.interval.Controls.Add(this.hrslbl);
            this.interval.Controls.Add(this.irvMins);
            this.interval.Controls.Add(this.irvHrs);
            this.interval.Font = new System.Drawing.Font("Microsoft Sans Serif", 8F);
            this.interval.Location = new System.Drawing.Point(12, 54);
            this.interval.Name = "interval";
            this.interval.Size = new System.Drawing.Size(468, 75);
            this.interval.TabIndex = 0;
            this.interval.TabStop = false;
            this.interval.Text = "Interval";
            // 
            // millislbl
            // 
            this.millislbl.AutoSize = true;
            this.millislbl.Location = new System.Drawing.Point(391, 39);
            this.millislbl.Name = "millislbl";
            this.millislbl.Size = new System.Drawing.Size(63, 13);
            this.millislbl.TabIndex = 7;
            this.millislbl.Text = "milliseconds";
            // 
            // irvMillis
            // 
            this.irvMillis.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.irvMillis.ForeColor = System.Drawing.Color.White;
            this.irvMillis.Location = new System.Drawing.Point(337, 36);
            this.irvMillis.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvMillis.Name = "irvMillis";
            this.irvMillis.Size = new System.Drawing.Size(51, 20);
            this.irvMillis.TabIndex = 6;
            // 
            // seclbl
            // 
            this.seclbl.AutoSize = true;
            this.seclbl.Location = new System.Drawing.Point(278, 39);
            this.seclbl.Name = "seclbl";
            this.seclbl.Size = new System.Drawing.Size(47, 13);
            this.seclbl.TabIndex = 5;
            this.seclbl.Text = "seconds";
            // 
            // irvSecs
            // 
            this.irvSecs.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.irvSecs.ForeColor = System.Drawing.Color.White;
            this.irvSecs.Location = new System.Drawing.Point(224, 36);
            this.irvSecs.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvSecs.Name = "irvSecs";
            this.irvSecs.Size = new System.Drawing.Size(51, 20);
            this.irvSecs.TabIndex = 4;
            // 
            // minlbl
            // 
            this.minlbl.AutoSize = true;
            this.minlbl.Location = new System.Drawing.Point(172, 39);
            this.minlbl.Name = "minlbl";
            this.minlbl.Size = new System.Drawing.Size(43, 13);
            this.minlbl.TabIndex = 3;
            this.minlbl.Text = "minutes";
            // 
            // hrslbl
            // 
            this.hrslbl.AutoSize = true;
            this.hrslbl.Location = new System.Drawing.Point(70, 39);
            this.hrslbl.Name = "hrslbl";
            this.hrslbl.Size = new System.Drawing.Size(33, 13);
            this.hrslbl.TabIndex = 2;
            this.hrslbl.Text = "hours";
            // 
            // irvMins
            // 
            this.irvMins.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.irvMins.ForeColor = System.Drawing.Color.White;
            this.irvMins.Location = new System.Drawing.Point(116, 36);
            this.irvMins.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvMins.Name = "irvMins";
            this.irvMins.Size = new System.Drawing.Size(51, 20);
            this.irvMins.TabIndex = 1;
            // 
            // irvHrs
            // 
            this.irvHrs.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.irvHrs.ForeColor = System.Drawing.Color.White;
            this.irvHrs.Location = new System.Drawing.Point(16, 36);
            this.irvHrs.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvHrs.Name = "irvHrs";
            this.irvHrs.Size = new System.Drawing.Size(51, 20);
            this.irvHrs.TabIndex = 0;
            // 
            // repeat
            // 
            this.repeat.Controls.Add(this.timeslbl);
            this.repeat.Controls.Add(this.repeatTimes);
            this.repeat.Controls.Add(this.repeatForeverBtn);
            this.repeat.Controls.Add(this.repeatTimesBtn);
            this.repeat.Location = new System.Drawing.Point(12, 235);
            this.repeat.Name = "repeat";
            this.repeat.Size = new System.Drawing.Size(224, 131);
            this.repeat.TabIndex = 1;
            this.repeat.TabStop = false;
            this.repeat.Text = "Repeat";
            // 
            // timeslbl
            // 
            this.timeslbl.AutoSize = true;
            this.timeslbl.Location = new System.Drawing.Point(171, 37);
            this.timeslbl.Name = "timeslbl";
            this.timeslbl.Size = new System.Drawing.Size(31, 13);
            this.timeslbl.TabIndex = 8;
            this.timeslbl.Text = "times";
            // 
            // repeatTimes
            // 
            this.repeatTimes.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.repeatTimes.ForeColor = System.Drawing.Color.White;
            this.repeatTimes.Location = new System.Drawing.Point(111, 33);
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
            this.repeatTimes.Size = new System.Drawing.Size(51, 20);
            this.repeatTimes.TabIndex = 11;
            this.repeatTimes.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // repeatForeverBtn
            // 
            this.repeatForeverBtn.AutoSize = true;
            this.repeatForeverBtn.Location = new System.Drawing.Point(18, 72);
            this.repeatForeverBtn.Name = "repeatForeverBtn";
            this.repeatForeverBtn.Size = new System.Drawing.Size(121, 17);
            this.repeatForeverBtn.TabIndex = 1;
            this.repeatForeverBtn.TabStop = true;
            this.repeatForeverBtn.Text = "Repat Until Stopped";
            this.repeatForeverBtn.UseVisualStyleBackColor = true;
            // 
            // repeatTimesBtn
            // 
            this.repeatTimesBtn.AutoSize = true;
            this.repeatTimesBtn.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(192)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.repeatTimesBtn.Location = new System.Drawing.Point(16, 33);
            this.repeatTimesBtn.Name = "repeatTimesBtn";
            this.repeatTimesBtn.Size = new System.Drawing.Size(54, 17);
            this.repeatTimesBtn.TabIndex = 0;
            this.repeatTimesBtn.TabStop = true;
            this.repeatTimesBtn.Text = "Repat";
            this.repeatTimesBtn.UseVisualStyleBackColor = true;
            // 
            // options
            // 
            this.options.Controls.Add(this.langBtn);
            this.options.Controls.Add(this.langlbl);
            this.options.Controls.Add(this.clickTypeBtn);
            this.options.Controls.Add(this.msButtonBtn);
            this.options.Controls.Add(this.clicktypelbl);
            this.options.Controls.Add(this.msbtnlbl);
            this.options.Location = new System.Drawing.Point(256, 235);
            this.options.Name = "options";
            this.options.Size = new System.Drawing.Size(224, 131);
            this.options.TabIndex = 1;
            this.options.TabStop = false;
            this.options.Text = "Options";
            // 
            // langBtn
            // 
            this.langBtn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.langBtn.FormattingEnabled = true;
            this.langBtn.Items.AddRange(new object[] {
            "English",
            "Türkçe",
            "Deutsch",
            "Français"});
            this.langBtn.Location = new System.Drawing.Point(103, 97);
            this.langBtn.Name = "langBtn";
            this.langBtn.Size = new System.Drawing.Size(105, 21);
            this.langBtn.TabIndex = 13;
            this.langBtn.SelectedIndexChanged += new System.EventHandler(this.langBtn_SelectedIndexChanged);
            // 
            // langlbl
            // 
            this.langlbl.AutoSize = true;
            this.langlbl.Location = new System.Drawing.Point(15, 99);
            this.langlbl.Name = "langlbl";
            this.langlbl.Size = new System.Drawing.Size(55, 13);
            this.langlbl.TabIndex = 12;
            this.langlbl.Text = "Language";
            // 
            // clickTypeBtn
            // 
            this.clickTypeBtn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.clickTypeBtn.FormattingEnabled = true;
            this.clickTypeBtn.Items.AddRange(new object[] {
            "Single",
            "Double",
            "Triple",
            "Quadruple"});
            this.clickTypeBtn.Location = new System.Drawing.Point(103, 63);
            this.clickTypeBtn.Name = "clickTypeBtn";
            this.clickTypeBtn.Size = new System.Drawing.Size(105, 21);
            this.clickTypeBtn.TabIndex = 11;
            // 
            // msButtonBtn
            // 
            this.msButtonBtn.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.msButtonBtn.FormattingEnabled = true;
            this.msButtonBtn.Items.AddRange(new object[] {
            "Left",
            "Right",
            "Mid"});
            this.msButtonBtn.Location = new System.Drawing.Point(103, 29);
            this.msButtonBtn.Name = "msButtonBtn";
            this.msButtonBtn.Size = new System.Drawing.Size(105, 21);
            this.msButtonBtn.TabIndex = 10;
            // 
            // clicktypelbl
            // 
            this.clicktypelbl.AutoSize = true;
            this.clicktypelbl.Location = new System.Drawing.Point(14, 65);
            this.clicktypelbl.Name = "clicktypelbl";
            this.clicktypelbl.Size = new System.Drawing.Size(57, 13);
            this.clicktypelbl.TabIndex = 9;
            this.clicktypelbl.Text = "Click Type";
            // 
            // msbtnlbl
            // 
            this.msbtnlbl.AutoSize = true;
            this.msbtnlbl.Location = new System.Drawing.Point(14, 32);
            this.msbtnlbl.Name = "msbtnlbl";
            this.msbtnlbl.Size = new System.Drawing.Size(73, 13);
            this.msbtnlbl.TabIndex = 8;
            this.msbtnlbl.Text = "Mouse Button";
            // 
            // position
            // 
            this.position.Controls.Add(this.label8);
            this.position.Controls.Add(this.label7);
            this.position.Controls.Add(this.posY);
            this.position.Controls.Add(this.posX);
            this.position.Controls.Add(this.specPosBtn);
            this.position.Controls.Add(this.currentPosBtn);
            this.position.Location = new System.Drawing.Point(12, 135);
            this.position.Name = "position";
            this.position.Size = new System.Drawing.Size(468, 94);
            this.position.TabIndex = 2;
            this.position.TabStop = false;
            this.position.Text = "Position";
            // 
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(369, 25);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(14, 13);
            this.label8.TabIndex = 10;
            this.label8.Text = "Y";
            // 
            // label7
            // 
            this.label7.AutoSize = true;
            this.label7.Location = new System.Drawing.Point(288, 25);
            this.label7.Name = "label7";
            this.label7.Size = new System.Drawing.Size(14, 13);
            this.label7.TabIndex = 8;
            this.label7.Text = "X";
            // 
            // posY
            // 
            this.posY.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.posY.ForeColor = System.Drawing.Color.White;
            this.posY.Location = new System.Drawing.Point(369, 40);
            this.posY.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.posY.Name = "posY";
            this.posY.Size = new System.Drawing.Size(51, 20);
            this.posY.TabIndex = 9;
            this.posY.ThousandsSeparator = true;
            // 
            // posX
            // 
            this.posX.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.posX.ForeColor = System.Drawing.Color.White;
            this.posX.Location = new System.Drawing.Point(287, 40);
            this.posX.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.posX.Name = "posX";
            this.posX.Size = new System.Drawing.Size(51, 20);
            this.posX.TabIndex = 8;
            this.posX.ThousandsSeparator = true;
            // 
            // specPosBtn
            // 
            this.specPosBtn.AutoSize = true;
            this.specPosBtn.Location = new System.Drawing.Point(168, 40);
            this.specPosBtn.Name = "specPosBtn";
            this.specPosBtn.Size = new System.Drawing.Size(113, 17);
            this.specPosBtn.TabIndex = 1;
            this.specPosBtn.TabStop = true;
            this.specPosBtn.Text = "Specified Location";
            this.specPosBtn.UseVisualStyleBackColor = true;
            // 
            // currentPosBtn
            // 
            this.currentPosBtn.AutoSize = true;
            this.currentPosBtn.Location = new System.Drawing.Point(18, 40);
            this.currentPosBtn.Name = "currentPosBtn";
            this.currentPosBtn.Size = new System.Drawing.Size(99, 17);
            this.currentPosBtn.TabIndex = 0;
            this.currentPosBtn.TabStop = true;
            this.currentPosBtn.Text = "Current Position";
            this.currentPosBtn.UseVisualStyleBackColor = true;
            // 
            // startBtn
            // 
            this.startBtn.BackColor = System.Drawing.Color.DarkGreen;
            this.startBtn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.startBtn.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(64)))), ((int)(((byte)(0)))));
            this.startBtn.FlatAppearance.BorderSize = 2;
            this.startBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.startBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.startBtn.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.startBtn.Location = new System.Drawing.Point(28, 372);
            this.startBtn.Name = "startBtn";
            this.startBtn.Size = new System.Drawing.Size(128, 56);
            this.startBtn.TabIndex = 3;
            this.startBtn.Text = "START ($HTK)";
            this.startBtn.UseVisualStyleBackColor = false;
            this.startBtn.Click += new System.EventHandler(this.startBtn_Click);
            // 
            // stopBtn
            // 
            this.stopBtn.BackColor = System.Drawing.Color.Maroon;
            this.stopBtn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.stopBtn.Enabled = false;
            this.stopBtn.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(64)))), ((int)(((byte)(0)))), ((int)(((byte)(0)))));
            this.stopBtn.FlatAppearance.BorderSize = 2;
            this.stopBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.stopBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.stopBtn.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.stopBtn.Location = new System.Drawing.Point(180, 372);
            this.stopBtn.Name = "stopBtn";
            this.stopBtn.Size = new System.Drawing.Size(128, 56);
            this.stopBtn.TabIndex = 4;
            this.stopBtn.Text = "STOP ($HTK)";
            this.stopBtn.UseVisualStyleBackColor = false;
            this.stopBtn.Click += new System.EventHandler(this.stopBtn_Click);
            // 
            // setHtkBtn
            // 
            this.setHtkBtn.BackColor = System.Drawing.SystemColors.HotTrack;
            this.setHtkBtn.Cursor = System.Windows.Forms.Cursors.Hand;
            this.setHtkBtn.FlatAppearance.BorderColor = System.Drawing.Color.FromArgb(((int)(((byte)(0)))), ((int)(((byte)(0)))), ((int)(((byte)(64)))));
            this.setHtkBtn.FlatAppearance.BorderSize = 2;
            this.setHtkBtn.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.setHtkBtn.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F, System.Drawing.FontStyle.Bold, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.setHtkBtn.ForeColor = System.Drawing.Color.WhiteSmoke;
            this.setHtkBtn.Location = new System.Drawing.Point(327, 372);
            this.setHtkBtn.Name = "setHtkBtn";
            this.setHtkBtn.Size = new System.Drawing.Size(128, 56);
            this.setHtkBtn.TabIndex = 5;
            this.setHtkBtn.Text = "SET HOTKEY";
            this.setHtkBtn.UseVisualStyleBackColor = false;
            this.setHtkBtn.Click += new System.EventHandler(this.setHtkBtn_Click);
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
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Font = new System.Drawing.Font("Microsoft YaHei", 8.25F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(162)));
            this.label1.ForeColor = System.Drawing.SystemColors.ButtonShadow;
            this.label1.Location = new System.Drawing.Point(385, 439);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(103, 16);
            this.label1.TabIndex = 10;
            this.label1.Text = "ilpeN © 2025, v1.2";
            // 
            // MainMenu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(20)))), ((int)(((byte)(20)))));
            this.ClientSize = new System.Drawing.Size(492, 461);
            this.Controls.Add(this.label1);
            this.Controls.Add(this.activemacrolbl);
            this.Controls.Add(this.setHtkBtn);
            this.Controls.Add(this.stopBtn);
            this.Controls.Add(this.startBtn);
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
            ((System.ComponentModel.ISupportInitialize)(this.irvMillis)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvSecs)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvMins)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvHrs)).EndInit();
            this.repeat.ResumeLayout(false);
            this.repeat.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.repeatTimes)).EndInit();
            this.options.ResumeLayout(false);
            this.options.PerformLayout();
            this.position.ResumeLayout(false);
            this.position.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.posY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.posX)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox interval;
        private System.Windows.Forms.GroupBox repeat;
        private System.Windows.Forms.GroupBox options;
        private System.Windows.Forms.GroupBox position;
        private System.Windows.Forms.NumericUpDown irvHrs;
        private System.Windows.Forms.Label millislbl;
        private System.Windows.Forms.NumericUpDown irvMillis;
        private System.Windows.Forms.Label seclbl;
        private System.Windows.Forms.NumericUpDown irvSecs;
        private System.Windows.Forms.Label minlbl;
        private System.Windows.Forms.Label hrslbl;
        private System.Windows.Forms.NumericUpDown irvMins;
        private System.Windows.Forms.Label msbtnlbl;
        private System.Windows.Forms.ComboBox clickTypeBtn;
        private System.Windows.Forms.ComboBox msButtonBtn;
        private System.Windows.Forms.Label clicktypelbl;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.NumericUpDown posY;
        private System.Windows.Forms.NumericUpDown posX;
        private System.Windows.Forms.RadioButton specPosBtn;
        private System.Windows.Forms.RadioButton currentPosBtn;
        private System.Windows.Forms.RadioButton repeatForeverBtn;
        private System.Windows.Forms.RadioButton repeatTimesBtn;
        private System.Windows.Forms.Label timeslbl;
        private System.Windows.Forms.NumericUpDown repeatTimes;
        private System.Windows.Forms.Button startBtn;
        private System.Windows.Forms.Button stopBtn;
        private System.Windows.Forms.Button setHtkBtn;
        private System.Windows.Forms.ComboBox langBtn;
        private System.Windows.Forms.Label langlbl;
        private System.Windows.Forms.Label activemacrolbl;
        private System.Windows.Forms.Label label1;
    }
}

