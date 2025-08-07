namespace AutoClicker
{
    partial class MacroForm
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
            System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MacroForm));
            this.label1 = new System.Windows.Forms.Label();
            this.list = new System.Windows.Forms.ListView();
            this.errlbl = new System.Windows.Forms.Label();
            this.createBtn = new Krypton.Toolkit.KryptonButton();
            this.deleteBtn = new Krypton.Toolkit.KryptonButton();
            this.selectBtn = new Krypton.Toolkit.KryptonButton();
            this.cancelBtn = new Krypton.Toolkit.KryptonButton();
            this.SuspendLayout();
            // 
            // label1
            // 
            this.label1.AutoSize = true;
            this.label1.Location = new System.Drawing.Point(12, 26);
            this.label1.Name = "label1";
            this.label1.Size = new System.Drawing.Size(73, 17);
            this.label1.TabIndex = 0;
            this.label1.Text = "Macro List";
            // 
            // list
            // 
            this.list.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(20)))), ((int)(((byte)(20)))));
            this.list.ForeColor = System.Drawing.Color.White;
            this.list.FullRowSelect = true;
            this.list.HideSelection = false;
            this.list.Location = new System.Drawing.Point(9, 46);
            this.list.MultiSelect = false;
            this.list.Name = "list";
            this.list.OwnerDraw = true;
            this.list.Size = new System.Drawing.Size(488, 260);
            this.list.TabIndex = 7;
            this.list.UseCompatibleStateImageBehavior = false;
            this.list.View = System.Windows.Forms.View.Details;
            // 
            // errlbl
            // 
            this.errlbl.AutoSize = true;
            this.errlbl.ForeColor = System.Drawing.Color.FromArgb(((int)(((byte)(214)))), ((int)(((byte)(89)))), ((int)(((byte)(77)))));
            this.errlbl.Location = new System.Drawing.Point(234, 355);
            this.errlbl.Name = "errlbl";
            this.errlbl.Size = new System.Drawing.Size(40, 17);
            this.errlbl.TabIndex = 8;
            this.errlbl.Text = "errlbl";
            this.errlbl.Visible = false;
            // 
            // createBtn
            // 
            this.createBtn.Location = new System.Drawing.Point(9, 312);
            this.createBtn.Name = "createBtn";
            this.createBtn.Size = new System.Drawing.Size(117, 36);
            this.createBtn.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(230)))), ((int)(((byte)(150)))));
            this.createBtn.StateNormal.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(200)))), ((int)(((byte)(90)))));
            this.createBtn.StateNormal.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(170)))), ((int)(((byte)(80)))));
            this.createBtn.StateNormal.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(170)))), ((int)(((byte)(80)))));
            this.createBtn.StateNormal.Border.Rounding = 5F;
            this.createBtn.StateNormal.Border.Width = 2;
            this.createBtn.StatePressed.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(110)))), ((int)(((byte)(190)))), ((int)(((byte)(110)))));
            this.createBtn.StatePressed.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(160)))), ((int)(((byte)(70)))));
            this.createBtn.StatePressed.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(140)))), ((int)(((byte)(60)))));
            this.createBtn.StatePressed.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(140)))), ((int)(((byte)(60)))));
            this.createBtn.StatePressed.Border.Rounding = 5F;
            this.createBtn.StatePressed.Border.Width = 2;
            this.createBtn.StateTracking.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(250)))), ((int)(((byte)(180)))));
            this.createBtn.StateTracking.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(220)))), ((int)(((byte)(120)))));
            this.createBtn.StateTracking.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(200)))), ((int)(((byte)(100)))));
            this.createBtn.StateTracking.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(200)))), ((int)(((byte)(100)))));
            this.createBtn.StateTracking.Border.Rounding = 5F;
            this.createBtn.StateTracking.Border.Width = 2;
            this.createBtn.TabIndex = 18;
            this.createBtn.Values.DropDownArrowColor = System.Drawing.Color.Empty;
            this.createBtn.Values.Text = "CREATE";
            this.createBtn.Click += new System.EventHandler(this.createBtn_Click);
            // 
            // deleteBtn
            // 
            this.deleteBtn.Location = new System.Drawing.Point(132, 312);
            this.deleteBtn.Name = "deleteBtn";
            this.deleteBtn.Size = new System.Drawing.Size(117, 36);
            this.deleteBtn.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(140)))), ((int)(((byte)(140)))));
            this.deleteBtn.StateNormal.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.deleteBtn.StateNormal.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.deleteBtn.StateNormal.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.deleteBtn.StateNormal.Border.Rounding = 5F;
            this.deleteBtn.StateNormal.Border.Width = 2;
            this.deleteBtn.StatePressed.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(210)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.deleteBtn.StatePressed.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(160)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.deleteBtn.StatePressed.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.deleteBtn.StatePressed.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.deleteBtn.StatePressed.Border.Rounding = 5F;
            this.deleteBtn.StatePressed.Border.Width = 2;
            this.deleteBtn.StateTracking.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(160)))), ((int)(((byte)(160)))));
            this.deleteBtn.StateTracking.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
            this.deleteBtn.StateTracking.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(220)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.deleteBtn.StateTracking.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(220)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.deleteBtn.StateTracking.Border.Rounding = 5F;
            this.deleteBtn.StateTracking.Border.Width = 2;
            this.deleteBtn.TabIndex = 17;
            this.deleteBtn.Values.DropDownArrowColor = System.Drawing.Color.Empty;
            this.deleteBtn.Values.Text = "DELETE";
            this.deleteBtn.Click += new System.EventHandler(this.deleteBtn_Click);
            // 
            // selectBtn
            // 
            this.selectBtn.Location = new System.Drawing.Point(255, 312);
            this.selectBtn.Name = "selectBtn";
            this.selectBtn.Size = new System.Drawing.Size(117, 36);
            this.selectBtn.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(230)))), ((int)(((byte)(150)))));
            this.selectBtn.StateNormal.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(90)))), ((int)(((byte)(200)))), ((int)(((byte)(90)))));
            this.selectBtn.StateNormal.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(170)))), ((int)(((byte)(80)))));
            this.selectBtn.StateNormal.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(80)))), ((int)(((byte)(170)))), ((int)(((byte)(80)))));
            this.selectBtn.StateNormal.Border.Rounding = 5F;
            this.selectBtn.StateNormal.Border.Width = 2;
            this.selectBtn.StatePressed.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(110)))), ((int)(((byte)(190)))), ((int)(((byte)(110)))));
            this.selectBtn.StatePressed.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(70)))), ((int)(((byte)(160)))), ((int)(((byte)(70)))));
            this.selectBtn.StatePressed.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(140)))), ((int)(((byte)(60)))));
            this.selectBtn.StatePressed.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(60)))), ((int)(((byte)(140)))), ((int)(((byte)(60)))));
            this.selectBtn.StatePressed.Border.Rounding = 5F;
            this.selectBtn.StatePressed.Border.Width = 2;
            this.selectBtn.StateTracking.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(180)))), ((int)(((byte)(250)))), ((int)(((byte)(180)))));
            this.selectBtn.StateTracking.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(120)))), ((int)(((byte)(220)))), ((int)(((byte)(120)))));
            this.selectBtn.StateTracking.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(200)))), ((int)(((byte)(100)))));
            this.selectBtn.StateTracking.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(100)))), ((int)(((byte)(200)))), ((int)(((byte)(100)))));
            this.selectBtn.StateTracking.Border.Rounding = 5F;
            this.selectBtn.StateTracking.Border.Width = 2;
            this.selectBtn.TabIndex = 20;
            this.selectBtn.Values.DropDownArrowColor = System.Drawing.Color.Empty;
            this.selectBtn.Values.Text = "SELECT";
            this.selectBtn.Click += new System.EventHandler(this.saveBtn_Click);
            // 
            // cancelBtn
            // 
            this.cancelBtn.Location = new System.Drawing.Point(378, 312);
            this.cancelBtn.Name = "cancelBtn";
            this.cancelBtn.Size = new System.Drawing.Size(117, 36);
            this.cancelBtn.StateNormal.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(140)))), ((int)(((byte)(140)))));
            this.cancelBtn.StateNormal.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(230)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.cancelBtn.StateNormal.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.cancelBtn.StateNormal.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(200)))), ((int)(((byte)(60)))), ((int)(((byte)(60)))));
            this.cancelBtn.StateNormal.Border.Rounding = 5F;
            this.cancelBtn.StateNormal.Border.Width = 2;
            this.cancelBtn.StatePressed.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(210)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.cancelBtn.StatePressed.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(160)))), ((int)(((byte)(50)))), ((int)(((byte)(50)))));
            this.cancelBtn.StatePressed.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.cancelBtn.StatePressed.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(150)))), ((int)(((byte)(40)))), ((int)(((byte)(40)))));
            this.cancelBtn.StatePressed.Border.Rounding = 5F;
            this.cancelBtn.StatePressed.Border.Width = 2;
            this.cancelBtn.StateTracking.Back.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(255)))), ((int)(((byte)(160)))), ((int)(((byte)(160)))));
            this.cancelBtn.StateTracking.Back.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(240)))), ((int)(((byte)(100)))), ((int)(((byte)(100)))));
            this.cancelBtn.StateTracking.Border.Color1 = System.Drawing.Color.FromArgb(((int)(((byte)(220)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.cancelBtn.StateTracking.Border.Color2 = System.Drawing.Color.FromArgb(((int)(((byte)(220)))), ((int)(((byte)(80)))), ((int)(((byte)(80)))));
            this.cancelBtn.StateTracking.Border.Rounding = 5F;
            this.cancelBtn.StateTracking.Border.Width = 2;
            this.cancelBtn.TabIndex = 19;
            this.cancelBtn.Values.DropDownArrowColor = System.Drawing.Color.Empty;
            this.cancelBtn.Values.Text = "CANCEL";
            this.cancelBtn.Click += new System.EventHandler(this.cancelBtn_Click);
            // 
            // MacroForm
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(8F, 16F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackColor = System.Drawing.Color.FromArgb(((int)(((byte)(20)))), ((int)(((byte)(20)))), ((int)(((byte)(20)))));
            this.ClientSize = new System.Drawing.Size(511, 381);
            this.Controls.Add(this.selectBtn);
            this.Controls.Add(this.cancelBtn);
            this.Controls.Add(this.createBtn);
            this.Controls.Add(this.deleteBtn);
            this.Controls.Add(this.errlbl);
            this.Controls.Add(this.list);
            this.Controls.Add(this.label1);
            this.Font = new System.Drawing.Font("Microsoft Sans Serif", 10F);
            this.ForeColor = System.Drawing.Color.White;
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
            this.Margin = new System.Windows.Forms.Padding(4);
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "MacroForm";
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
            this.Text = "Macro Selection";
            this.Load += new System.EventHandler(this.MacroForm_Load);
            this.Controls.SetChildIndex(this.label1, 0);
            this.Controls.SetChildIndex(this.list, 0);
            this.Controls.SetChildIndex(this.errlbl, 0);
            this.Controls.SetChildIndex(this.deleteBtn, 0);
            this.Controls.SetChildIndex(this.createBtn, 0);
            this.Controls.SetChildIndex(this.cancelBtn, 0);
            this.Controls.SetChildIndex(this.selectBtn, 0);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private System.Windows.Forms.Label label1;
        private System.Windows.Forms.ListView list;
        private System.Windows.Forms.Label errlbl;
        private Krypton.Toolkit.KryptonButton createBtn;
        private Krypton.Toolkit.KryptonButton deleteBtn;
        private Krypton.Toolkit.KryptonButton selectBtn;
        private Krypton.Toolkit.KryptonButton cancelBtn;
    }
}