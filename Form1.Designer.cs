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
            this.repeat = new System.Windows.Forms.GroupBox();
            this.options = new System.Windows.Forms.GroupBox();
            this.position = new System.Windows.Forms.GroupBox();
            this.irvHrs = new System.Windows.Forms.NumericUpDown();
            this.irvMins = new System.Windows.Forms.NumericUpDown();
            this.label1 = new System.Windows.Forms.Label();
            this.label2 = new System.Windows.Forms.Label();
            this.label3 = new System.Windows.Forms.Label();
            this.irvSecs = new System.Windows.Forms.NumericUpDown();
            this.label4 = new System.Windows.Forms.Label();
            this.irvMillis = new System.Windows.Forms.NumericUpDown();
            this.label5 = new System.Windows.Forms.Label();
            this.label6 = new System.Windows.Forms.Label();
            this.msButtonBtn = new System.Windows.Forms.ComboBox();
            this.clickTypeBtn = new System.Windows.Forms.ComboBox();
            this.currentPosBtn = new System.Windows.Forms.RadioButton();
            this.specPosBtn = new System.Windows.Forms.RadioButton();
            this.posX = new System.Windows.Forms.NumericUpDown();
            this.posY = new System.Windows.Forms.NumericUpDown();
            this.label7 = new System.Windows.Forms.Label();
            this.label8 = new System.Windows.Forms.Label();
            this.repeatTimesBtn = new System.Windows.Forms.RadioButton();
            this.repeatForeverBtn = new System.Windows.Forms.RadioButton();
            this.repeatTimes = new System.Windows.Forms.NumericUpDown();
            this.label9 = new System.Windows.Forms.Label();
            this.startBtn = new System.Windows.Forms.Button();
            this.stopBtn = new System.Windows.Forms.Button();
            this.setHtkBtn = new System.Windows.Forms.Button();
            this.cancelSettingHotkey = new System.Windows.Forms.Label();
            this.interval.SuspendLayout();
            this.repeat.SuspendLayout();
            this.options.SuspendLayout();
            this.position.SuspendLayout();
            ((System.ComponentModel.ISupportInitialize)(this.irvHrs)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvMins)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvSecs)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvMillis)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.posX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.posY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.repeatTimes)).BeginInit();
            this.SuspendLayout();
            // 
            // interval
            // 
            this.interval.Controls.Add(this.label4);
            this.interval.Controls.Add(this.irvMillis);
            this.interval.Controls.Add(this.label3);
            this.interval.Controls.Add(this.irvSecs);
            this.interval.Controls.Add(this.label2);
            this.interval.Controls.Add(this.label1);
            this.interval.Controls.Add(this.irvMins);
            this.interval.Controls.Add(this.irvHrs);
            this.interval.Location = new System.Drawing.Point(12, 12);
            this.interval.Name = "interval";
            this.interval.Size = new System.Drawing.Size(454, 75);
            this.interval.TabIndex = 0;
            this.interval.TabStop = false;
            this.interval.Text = "Interval";
            // 
            // repeat
            // 
            this.repeat.Controls.Add(this.label9);
            this.repeat.Controls.Add(this.repeatTimes);
            this.repeat.Controls.Add(this.repeatForeverBtn);
            this.repeat.Controls.Add(this.repeatTimesBtn);
            this.repeat.Location = new System.Drawing.Point(12, 193);
            this.repeat.Name = "repeat";
            this.repeat.Size = new System.Drawing.Size(224, 131);
            this.repeat.TabIndex = 1;
            this.repeat.TabStop = false;
            this.repeat.Text = "Repeat";
            // 
            // options
            // 
            this.options.Controls.Add(this.clickTypeBtn);
            this.options.Controls.Add(this.msButtonBtn);
            this.options.Controls.Add(this.label6);
            this.options.Controls.Add(this.label5);
            this.options.Location = new System.Drawing.Point(242, 193);
            this.options.Name = "options";
            this.options.Size = new System.Drawing.Size(224, 131);
            this.options.TabIndex = 1;
            this.options.TabStop = false;
            this.options.Text = "Options";
            // 
            // position
            // 
            this.position.Controls.Add(this.label8);
            this.position.Controls.Add(this.label7);
            this.position.Controls.Add(this.posY);
            this.position.Controls.Add(this.posX);
            this.position.Controls.Add(this.specPosBtn);
            this.position.Controls.Add(this.currentPosBtn);
            this.position.Location = new System.Drawing.Point(12, 93);
            this.position.Name = "position";
            this.position.Size = new System.Drawing.Size(454, 94);
            this.position.TabIndex = 2;
            this.position.TabStop = false;
            this.position.Text = "Position";
            // 
            // irvHrs
            // 
            this.irvHrs.Location = new System.Drawing.Point(16, 36);
            this.irvHrs.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvHrs.Name = "irvHrs";
            this.irvHrs.Size = new System.Drawing.Size(51, 20);
            this.irvHrs.TabIndex = 0;
            this.irvHrs.ValueChanged += new System.EventHandler(this.irvHrs_ValueChanged);
            // 
            // irvMins
            // 
            this.irvMins.Location = new System.Drawing.Point(111, 36);
            this.irvMins.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvMins.Name = "irvMins";
            this.irvMins.Size = new System.Drawing.Size(51, 20);
            this.irvMins.TabIndex = 1;
            this.irvMins.ValueChanged += new System.EventHandler(this.irvMins_ValueChanged);
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(70, 39);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(33, 13);
            this.label1.TabIndex = 2;
            this.label1.Text = "hours";
            // 
            // label2
            // 
            this.label2.AutoSize = true;
            this.label2.Location = new System.Drawing.Point(165, 39);
            this.label2.Name = "label2";
            this.label2.Size = new System.Drawing.Size(43, 13);
            this.label2.TabIndex = 3;
            this.label2.Text = "minutes";
            // 
            // label3
            // 
            this.label3.AutoSize = true;
            this.label3.Location = new System.Drawing.Point(271, 39);
            this.label3.Name = "label3";
            this.label3.Size = new System.Drawing.Size(47, 13);
            this.label3.TabIndex = 5;
            this.label3.Text = "seconds";
            // 
            // irvSecs
            // 
            this.irvSecs.Location = new System.Drawing.Point(217, 36);
            this.irvSecs.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvSecs.Name = "irvSecs";
            this.irvSecs.Size = new System.Drawing.Size(51, 20);
            this.irvSecs.TabIndex = 4;
            this.irvSecs.ValueChanged += new System.EventHandler(this.irvSecs_ValueChanged);
            // 
            // label4
            // 
            this.label4.AutoSize = true;
            this.label4.Location = new System.Drawing.Point(380, 39);
            this.label4.Name = "label4";
            this.label4.Size = new System.Drawing.Size(63, 13);
            this.label4.TabIndex = 7;
            this.label4.Text = "milliseconds";
            // 
            // irvMillis
            // 
            this.irvMillis.Location = new System.Drawing.Point(327, 36);
            this.irvMillis.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvMillis.Name = "irvMillis";
            this.irvMillis.Size = new System.Drawing.Size(51, 20);
            this.irvMillis.TabIndex = 6;
            this.irvMillis.ValueChanged += new System.EventHandler(this.irvMillis_ValueChanged);
            // 
            // label5
            // 
            this.label5.AutoSize = true;
            this.label5.Location = new System.Drawing.Point(14, 32);
            this.label5.Name = "label5";
            this.label5.Size = new System.Drawing.Size(73, 13);
            this.label5.TabIndex = 8;
            this.label5.Text = "Mouse Button";
            // 
            // label6
            // 
            this.label6.AutoSize = true;
            this.label6.Location = new System.Drawing.Point(14, 71);
            this.label6.Name = "label6";
            this.label6.Size = new System.Drawing.Size(57, 13);
            this.label6.TabIndex = 9;
            this.label6.Text = "Click Type";
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
            this.msButtonBtn.SelectedIndexChanged += new System.EventHandler(this.msButtonBtn_SelectedIndexChanged);
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
            this.clickTypeBtn.Location = new System.Drawing.Point(103, 68);
            this.clickTypeBtn.Name = "clickTypeBtn";
            this.clickTypeBtn.Size = new System.Drawing.Size(105, 21);
            this.clickTypeBtn.TabIndex = 11;
            this.clickTypeBtn.SelectedIndexChanged += new System.EventHandler(this.clickTypeBtn_SelectedIndexChanged);
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
            this.currentPosBtn.CheckedChanged += new System.EventHandler(this.currentPosBtn_CheckedChanged);
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
            // posX
            // 
            this.posX.Location = new System.Drawing.Point(287, 40);
            this.posX.Name = "posX";
            this.posX.Size = new System.Drawing.Size(51, 20);
            this.posX.TabIndex = 8;
            this.posX.ValueChanged += new System.EventHandler(this.posX_ValueChanged);
            // 
            // posY
            // 
            this.posY.Location = new System.Drawing.Point(369, 40);
            this.posY.Name = "posY";
            this.posY.Size = new System.Drawing.Size(51, 20);
            this.posY.TabIndex = 9;
            this.posY.ValueChanged += new System.EventHandler(this.posY_ValueChanged);
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
            // label8
            // 
            this.label8.AutoSize = true;
            this.label8.Location = new System.Drawing.Point(369, 25);
            this.label8.Name = "label8";
            this.label8.Size = new System.Drawing.Size(14, 13);
            this.label8.TabIndex = 10;
            this.label8.Text = "Y";
            // 
            // repeatTimesBtn
            // 
            this.repeatTimesBtn.AutoSize = true;
            this.repeatTimesBtn.Location = new System.Drawing.Point(16, 33);
            this.repeatTimesBtn.Name = "repeatTimesBtn";
            this.repeatTimesBtn.Size = new System.Drawing.Size(54, 17);
            this.repeatTimesBtn.TabIndex = 0;
            this.repeatTimesBtn.TabStop = true;
            this.repeatTimesBtn.Text = "Repat";
            this.repeatTimesBtn.UseVisualStyleBackColor = true;
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
            this.repeatForeverBtn.CheckedChanged += new System.EventHandler(this.repeatForeverBtn_CheckedChanged);
            // 
            // repeatTimes
            // 
            this.repeatTimes.Location = new System.Drawing.Point(111, 33);
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
            this.repeatTimes.ValueChanged += new System.EventHandler(this.repeatTimes_ValueChanged);
            // 
            // label9
            // 
            this.label9.AutoSize = true;
            this.label9.Location = new System.Drawing.Point(171, 37);
            this.label9.Name = "label9";
            this.label9.Size = new System.Drawing.Size(31, 13);
            this.label9.TabIndex = 8;
            this.label9.Text = "times";
            // 
            // startBtn
            // 
            this.startBtn.Location = new System.Drawing.Point(28, 330);
            this.startBtn.Name = "startBtn";
            this.startBtn.Size = new System.Drawing.Size(128, 56);
            this.startBtn.TabIndex = 3;
            this.startBtn.Text = "START ($HTK)";
            this.startBtn.UseVisualStyleBackColor = true;
            this.startBtn.Click += new System.EventHandler(this.startBtn_Click);
            // 
            // stopBtn
            // 
            this.stopBtn.Location = new System.Drawing.Point(180, 330);
            this.stopBtn.Name = "stopBtn";
            this.stopBtn.Size = new System.Drawing.Size(128, 56);
            this.stopBtn.TabIndex = 4;
            this.stopBtn.Text = "STOP ($HTK)";
            this.stopBtn.UseVisualStyleBackColor = true;
            this.stopBtn.Click += new System.EventHandler(this.stopBtn_Click);
            // 
            // setHtkBtn
            // 
            this.setHtkBtn.Location = new System.Drawing.Point(327, 330);
            this.setHtkBtn.Name = "setHtkBtn";
            this.setHtkBtn.Size = new System.Drawing.Size(128, 56);
            this.setHtkBtn.TabIndex = 5;
            this.setHtkBtn.Text = "SET HOTKEY";
            this.setHtkBtn.UseVisualStyleBackColor = true;
            this.setHtkBtn.Click += new System.EventHandler(this.setHtkBtn_Click);
            // 
            // cancelSettingHotkey
            // 
            this.cancelSettingHotkey.AutoSize = true;
            this.cancelSettingHotkey.Cursor = System.Windows.Forms.Cursors.Hand;
            this.cancelSettingHotkey.ForeColor = System.Drawing.Color.Maroon;
            this.cancelSettingHotkey.Location = new System.Drawing.Point(165, 400);
            this.cancelSettingHotkey.Name = "cancelSettingHotkey";
            this.cancelSettingHotkey.Size = new System.Drawing.Size(165, 13);
            this.cancelSettingHotkey.TabIndex = 8;
            this.cancelSettingHotkey.Text = "Click this to cancel setting hotkey";
            this.cancelSettingHotkey.Visible = false;
            this.cancelSettingHotkey.Click += new System.EventHandler(this.label10_Click);
            this.cancelSettingHotkey.MouseEnter += new System.EventHandler(this.cancelSettingHotkey_MouseEnter);
            this.cancelSettingHotkey.MouseLeave += new System.EventHandler(this.cancelSettingHotkey_MouseLeave);
            // 
            // MainMenu
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(492, 422);
            this.Controls.Add(this.cancelSettingHotkey);
            this.Controls.Add(this.setHtkBtn);
            this.Controls.Add(this.stopBtn);
            this.Controls.Add(this.startBtn);
            this.Controls.Add(this.position);
            this.Controls.Add(this.options);
            this.Controls.Add(this.repeat);
            this.Controls.Add(this.interval);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.Fixed3D;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.Name = "MainMenu";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Auto Clicker";
            this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.MainMenu_FormClosing);
            this.Load += new System.EventHandler(this.Form1_Load);
            this.KeyDown += new System.Windows.Forms.KeyEventHandler(this.MainMenu_KeyDown);
            this.interval.ResumeLayout(false);
            this.interval.PerformLayout();
            this.repeat.ResumeLayout(false);
            this.repeat.PerformLayout();
            this.options.ResumeLayout(false);
            this.options.PerformLayout();
            this.position.ResumeLayout(false);
            this.position.PerformLayout();
            ((System.ComponentModel.ISupportInitialize)(this.irvHrs)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvMins)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvSecs)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvMillis)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.posX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.posY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.repeatTimes)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.GroupBox interval;
        private System.Windows.Forms.GroupBox repeat;
        private System.Windows.Forms.GroupBox options;
        private System.Windows.Forms.GroupBox position;
        private System.Windows.Forms.NumericUpDown irvHrs;
        private System.Windows.Forms.Label label4;
        private System.Windows.Forms.NumericUpDown irvMillis;
        private System.Windows.Forms.Label label3;
        private System.Windows.Forms.NumericUpDown irvSecs;
        private System.Windows.Forms.Label label2;
        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.NumericUpDown irvMins;
        private System.Windows.Forms.Label label5;
        private System.Windows.Forms.ComboBox clickTypeBtn;
        private System.Windows.Forms.ComboBox msButtonBtn;
        private System.Windows.Forms.Label label6;
        private System.Windows.Forms.Label label8;
        private System.Windows.Forms.Label label7;
        private System.Windows.Forms.NumericUpDown posY;
        private System.Windows.Forms.NumericUpDown posX;
        private System.Windows.Forms.RadioButton specPosBtn;
        private System.Windows.Forms.RadioButton currentPosBtn;
        private System.Windows.Forms.RadioButton repeatForeverBtn;
        private System.Windows.Forms.RadioButton repeatTimesBtn;
        private System.Windows.Forms.Label label9;
        private System.Windows.Forms.NumericUpDown repeatTimes;
        private System.Windows.Forms.Button startBtn;
        private System.Windows.Forms.Button stopBtn;
        private System.Windows.Forms.Button setHtkBtn;
        private System.Windows.Forms.Label cancelSettingHotkey;
    }
}

