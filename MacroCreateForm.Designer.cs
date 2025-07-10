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

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private System.Windows.Forms.TextBox txtName;
        private System.Windows.Forms.TextBox txtDescription;
        private System.Windows.Forms.ComboBox cmbMouseButton;
        private System.Windows.Forms.ComboBox cmbClickType;
        private System.Windows.Forms.NumericUpDown posX;
        private System.Windows.Forms.NumericUpDown posY;
        private System.Windows.Forms.CheckBox chkUseCurrentPos;
        private System.Windows.Forms.Button btnSave;
        private System.Windows.Forms.Label lblName;
        private System.Windows.Forms.Label lblDesc;
        private System.Windows.Forms.Label lblMouse;
        private System.Windows.Forms.Label lblClick;
        private System.Windows.Forms.Label lblX;
        private System.Windows.Forms.Label lblY;

        private void InitializeComponent()
        {
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MacroCreateForm));
            this.txtName = new System.Windows.Forms.TextBox();
            this.txtDescription = new System.Windows.Forms.TextBox();
            this.cmbMouseButton = new System.Windows.Forms.ComboBox();
            this.cmbClickType = new System.Windows.Forms.ComboBox();
            this.posX = new System.Windows.Forms.NumericUpDown();
            this.posY = new System.Windows.Forms.NumericUpDown();
            this.chkUseCurrentPos = new System.Windows.Forms.CheckBox();
            this.btnSave = new System.Windows.Forms.Button();
            this.lblName = new System.Windows.Forms.Label();
            this.lblDesc = new System.Windows.Forms.Label();
            this.lblMouse = new System.Windows.Forms.Label();
            this.lblClick = new System.Windows.Forms.Label();
            this.lblX = new System.Windows.Forms.Label();
            this.lblY = new System.Windows.Forms.Label();
            this.repeatTimes = new System.Windows.Forms.NumericUpDown();
            this.repeattimeslbl = new System.Windows.Forms.Label();
            this.repeatForeverBtn = new System.Windows.Forms.CheckBox();
            this.intervallbl = new System.Windows.Forms.Label();
            this.millislbl = new System.Windows.Forms.Label();
            this.irvMillis = new System.Windows.Forms.NumericUpDown();
            this.seclbl = new System.Windows.Forms.Label();
            this.irvSecs = new System.Windows.Forms.NumericUpDown();
            this.minlbl = new System.Windows.Forms.Label();
            this.hrslbl = new System.Windows.Forms.Label();
            this.irvMins = new System.Windows.Forms.NumericUpDown();
            this.irvHrs = new System.Windows.Forms.NumericUpDown();
            ((System.ComponentModel.ISupportInitialize)(this.posX)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.posY)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.repeatTimes)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvMillis)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvSecs)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvMins)).BeginInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvHrs)).BeginInit();
            this.SuspendLayout();
            // 
            // txtName
            // 
            this.txtName.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.txtName.ForeColor = System.Drawing.Color.White;
            this.txtName.Location = new System.Drawing.Point(130, 15);
            this.txtName.Name = "txtName";
            this.txtName.Size = new System.Drawing.Size(200, 20);
            this.txtName.TabIndex = 0;
            // 
            // txtDescription
            // 
            this.txtDescription.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.txtDescription.ForeColor = System.Drawing.Color.White;
            this.txtDescription.Location = new System.Drawing.Point(130, 45);
            this.txtDescription.Name = "txtDescription";
            this.txtDescription.Size = new System.Drawing.Size(200, 20);
            this.txtDescription.TabIndex = 2;
            // 
            // cmbMouseButton
            // 
            this.cmbMouseButton.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.cmbMouseButton.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.cmbMouseButton.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbMouseButton.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F);
            this.cmbMouseButton.ForeColor = System.Drawing.Color.White;
            this.cmbMouseButton.Location = new System.Drawing.Point(130, 75);
            this.cmbMouseButton.Name = "cmbMouseButton";
            this.cmbMouseButton.Size = new System.Drawing.Size(200, 22);
            this.cmbMouseButton.TabIndex = 4;
            // 
            // cmbClickType
            // 
            this.cmbClickType.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.cmbClickType.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
            this.cmbClickType.DropDownStyle = System.Windows.Forms.ComboBoxStyle.DropDownList;
            this.cmbClickType.Font = new System.Drawing.Font("Microsoft Sans Serif", 9F);
            this.cmbClickType.ForeColor = System.Drawing.Color.White;
            this.cmbClickType.Location = new System.Drawing.Point(130, 105);
            this.cmbClickType.Name = "cmbClickType";
            this.cmbClickType.Size = new System.Drawing.Size(200, 22);
            this.cmbClickType.TabIndex = 6;
            // 
            // posX
            // 
            this.posX.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.posX.ForeColor = System.Drawing.Color.White;
            this.posX.Location = new System.Drawing.Point(130, 279);
            this.posX.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.posX.Name = "posX";
            this.posX.Size = new System.Drawing.Size(120, 20);
            this.posX.TabIndex = 9;
            // 
            // posY
            // 
            this.posY.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.posY.ForeColor = System.Drawing.Color.White;
            this.posY.Location = new System.Drawing.Point(130, 309);
            this.posY.Maximum = new decimal(new int[] {
            100000,
            0,
            0,
            0});
            this.posY.Name = "posY";
            this.posY.Size = new System.Drawing.Size(120, 20);
            this.posY.TabIndex = 11;
            // 
            // chkUseCurrentPos
            // 
            this.chkUseCurrentPos.Location = new System.Drawing.Point(130, 335);
            this.chkUseCurrentPos.Name = "chkUseCurrentPos";
            this.chkUseCurrentPos.Size = new System.Drawing.Size(200, 24);
            this.chkUseCurrentPos.TabIndex = 8;
            this.chkUseCurrentPos.Text = "Use Current Cursor Position";
            // 
            // btnSave
            // 
            this.btnSave.Cursor = System.Windows.Forms.Cursors.Hand;
            this.btnSave.FlatAppearance.BorderColor = System.Drawing.Color.DimGray;
            this.btnSave.FlatAppearance.BorderSize = 2;
            this.btnSave.FlatStyle = System.Windows.Forms.FlatStyle.Flat;
            this.btnSave.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.btnSave.Location = new System.Drawing.Point(259, 365);
            this.btnSave.Name = "btnSave";
            this.btnSave.Size = new System.Drawing.Size(99, 48);
            this.btnSave.TabIndex = 13;
            this.btnSave.Text = "Create";
            this.btnSave.Click += new System.EventHandler(this.btnSave_Click);
            // 
            // lblName
            // 
            this.lblName.Location = new System.Drawing.Point(12, 18);
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
            // lblMouse
            // 
            this.lblMouse.Location = new System.Drawing.Point(12, 78);
            this.lblMouse.Name = "lblMouse";
            this.lblMouse.Size = new System.Drawing.Size(100, 23);
            this.lblMouse.TabIndex = 5;
            this.lblMouse.Text = "Mouse Button:";
            // 
            // lblClick
            // 
            this.lblClick.Location = new System.Drawing.Point(12, 108);
            this.lblClick.Name = "lblClick";
            this.lblClick.Size = new System.Drawing.Size(100, 23);
            this.lblClick.TabIndex = 7;
            this.lblClick.Text = "Click Type:";
            // 
            // lblX
            // 
            this.lblX.Location = new System.Drawing.Point(12, 282);
            this.lblX.Name = "lblX";
            this.lblX.Size = new System.Drawing.Size(100, 23);
            this.lblX.TabIndex = 10;
            this.lblX.Text = "Position X:";
            // 
            // lblY
            // 
            this.lblY.Location = new System.Drawing.Point(12, 312);
            this.lblY.Name = "lblY";
            this.lblY.Size = new System.Drawing.Size(100, 23);
            this.lblY.TabIndex = 12;
            this.lblY.Text = "Position Y:";
            // 
            // repeatTimes
            // 
            this.repeatTimes.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.repeatTimes.ForeColor = System.Drawing.Color.White;
            this.repeatTimes.Location = new System.Drawing.Point(130, 217);
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
            this.repeatTimes.Size = new System.Drawing.Size(120, 20);
            this.repeatTimes.TabIndex = 14;
            this.repeatTimes.Value = new decimal(new int[] {
            1,
            0,
            0,
            0});
            // 
            // repeattimeslbl
            // 
            this.repeattimeslbl.Location = new System.Drawing.Point(12, 220);
            this.repeattimeslbl.Name = "repeattimeslbl";
            this.repeattimeslbl.Size = new System.Drawing.Size(100, 23);
            this.repeattimeslbl.TabIndex = 15;
            this.repeattimeslbl.Text = "Repeat:";
            // 
            // repeatForeverBtn
            // 
            this.repeatForeverBtn.Location = new System.Drawing.Point(130, 243);
            this.repeatForeverBtn.Name = "repeatForeverBtn";
            this.repeatForeverBtn.Size = new System.Drawing.Size(200, 24);
            this.repeatForeverBtn.TabIndex = 16;
            this.repeatForeverBtn.Text = "Repeat Until Stopped";
            // 
            // intervallbl
            // 
            this.intervallbl.Location = new System.Drawing.Point(12, 146);
            this.intervallbl.Name = "intervallbl";
            this.intervallbl.Size = new System.Drawing.Size(100, 23);
            this.intervallbl.TabIndex = 17;
            this.intervallbl.Text = "Interval:";
            // 
            // millislbl
            // 
            this.millislbl.AutoSize = true;
            this.millislbl.Location = new System.Drawing.Point(287, 183);
            this.millislbl.Name = "millislbl";
            this.millislbl.Size = new System.Drawing.Size(63, 13);
            this.millislbl.TabIndex = 26;
            this.millislbl.Text = "milliseconds";
            // 
            // irvMillis
            // 
            this.irvMillis.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.irvMillis.ForeColor = System.Drawing.Color.White;
            this.irvMillis.Location = new System.Drawing.Point(233, 180);
            this.irvMillis.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvMillis.Name = "irvMillis";
            this.irvMillis.Size = new System.Drawing.Size(51, 20);
            this.irvMillis.TabIndex = 25;
            // 
            // seclbl
            // 
            this.seclbl.AutoSize = true;
            this.seclbl.Location = new System.Drawing.Point(174, 183);
            this.seclbl.Name = "seclbl";
            this.seclbl.Size = new System.Drawing.Size(47, 13);
            this.seclbl.TabIndex = 24;
            this.seclbl.Text = "seconds";
            // 
            // irvSecs
            // 
            this.irvSecs.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.irvSecs.ForeColor = System.Drawing.Color.White;
            this.irvSecs.Location = new System.Drawing.Point(120, 180);
            this.irvSecs.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvSecs.Name = "irvSecs";
            this.irvSecs.Size = new System.Drawing.Size(51, 20);
            this.irvSecs.TabIndex = 23;
            // 
            // minlbl
            // 
            this.minlbl.AutoSize = true;
            this.minlbl.Location = new System.Drawing.Point(287, 146);
            this.minlbl.Name = "minlbl";
            this.minlbl.Size = new System.Drawing.Size(43, 13);
            this.minlbl.TabIndex = 22;
            this.minlbl.Text = "minutes";
            // 
            // hrslbl
            // 
            this.hrslbl.AutoSize = true;
            this.hrslbl.Location = new System.Drawing.Point(174, 146);
            this.hrslbl.Name = "hrslbl";
            this.hrslbl.Size = new System.Drawing.Size(33, 13);
            this.hrslbl.TabIndex = 21;
            this.hrslbl.Text = "hours";
            // 
            // irvMins
            // 
            this.irvMins.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.irvMins.ForeColor = System.Drawing.Color.White;
            this.irvMins.Location = new System.Drawing.Point(231, 143);
            this.irvMins.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvMins.Name = "irvMins";
            this.irvMins.Size = new System.Drawing.Size(51, 20);
            this.irvMins.TabIndex = 20;
            // 
            // irvHrs
            // 
            this.irvHrs.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(40)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.irvHrs.ForeColor = System.Drawing.Color.White;
            this.irvHrs.Location = new System.Drawing.Point(120, 143);
            this.irvHrs.Maximum = new decimal(new int[] {
            1000,
            0,
            0,
            0});
            this.irvHrs.Name = "irvHrs";
            this.irvHrs.Size = new System.Drawing.Size(51, 20);
            this.irvHrs.TabIndex = 19;
            // 
            // MacroCreateForm
            // 
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(20)))), ((int)(((byte)(20)))));
            this.ClientSize = new System.Drawing.Size(370, 425);
            this.Controls.Add(this.millislbl);
            this.Controls.Add(this.irvMillis);
            this.Controls.Add(this.seclbl);
            this.Controls.Add(this.irvSecs);
            this.Controls.Add(this.minlbl);
            this.Controls.Add(this.hrslbl);
            this.Controls.Add(this.irvMins);
            this.Controls.Add(this.irvHrs);
            this.Controls.Add(this.intervallbl);
            this.Controls.Add(this.repeatForeverBtn);
            this.Controls.Add(this.repeatTimes);
            this.Controls.Add(this.repeattimeslbl);
            this.Controls.Add(this.txtName);
            this.Controls.Add(this.lblName);
            this.Controls.Add(this.txtDescription);
            this.Controls.Add(this.lblDesc);
            this.Controls.Add(this.cmbMouseButton);
            this.Controls.Add(this.lblMouse);
            this.Controls.Add(this.cmbClickType);
            this.Controls.Add(this.lblClick);
            this.Controls.Add(this.chkUseCurrentPos);
            this.Controls.Add(this.posX);
            this.Controls.Add(this.lblX);
            this.Controls.Add(this.posY);
            this.Controls.Add(this.lblY);
            this.Controls.Add(this.btnSave);
            this.ForeColor = System.Drawing.Color.White;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MacroCreateForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Create New Macro";
            this.Load += new System.EventHandler(this.MacroCreateForm_Load);
            ((System.ComponentModel.ISupportInitialize)(this.posX)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.posY)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.repeatTimes)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvMillis)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvSecs)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvMins)).EndInit();
            ((System.ComponentModel.ISupportInitialize)(this.irvHrs)).EndInit();
            this.ResumeLayout(false);
            this.PerformLayout();

        }


        #endregion

        private NumericUpDown repeatTimes;
        private Label repeattimeslbl;
        private CheckBox repeatForeverBtn;
        private Label intervallbl;
        private Label millislbl;
        private NumericUpDown irvMillis;
        private Label seclbl;
        private NumericUpDown irvSecs;
        private Label minlbl;
        private Label hrslbl;
        private NumericUpDown irvMins;
        private NumericUpDown irvHrs;
    }
}