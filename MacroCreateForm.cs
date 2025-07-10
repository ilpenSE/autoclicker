using AutoClicker.Properties;
using System;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

namespace AutoClicker
{
    public partial class MacroCreateForm : Form
    {
        public MacroModel CreatedMacro { get; private set; }

        public MacroCreateForm()
        {
            InitializeComponent();
            cmbMouseButton.DataSource = new string[] {
                Resources.opt_mb_left,
                Resources.opt_mb_right,
                Resources.opt_mb_mid
            };
            cmbClickType.DataSource = new string[] {
                Resources.opt_ct_single,
                Resources.opt_ct_double,
                Resources.opt_ct_triple,
                Resources.opt_ct_quadruple
            };

            LoadLanguageValues();
        }

        private void LoadLanguageValues()
        {
            lblName.Text = Resources.lblname + ":";
            lblDesc.Text = Resources.lbldesc + ":";
            lblMouse.Text = Resources.opt_msbtn + ":";
            lblClick.Text = Resources.opt_clicktype + ":";
            intervallbl.Text = Resources.interval_groupbox + ":";
            hrslbl.Text = Resources.interval_hours;
            minlbl.Text = Resources.interval_mins;
            seclbl.Text = Resources.interval_secs;
            millislbl.Text = Resources.interval_millis;
            lblX.Text = Resources.pos_gb + " X:";
            lblY.Text = Resources.pos_gb + " Y:";
            chkUseCurrentPos.Text = Resources.pos_current;
            repeatForeverBtn.Text = Resources.rep_forever;
            repeattimeslbl.Text = Resources.rep_gb + ":";
            this.Text = Resources.createmacro_title;
            btnSave.Text = MainMenu.CapitalizeFirstLetter(Resources.create);
        }

        private int GetIntervalInMillis()
        {
            int total = 0;
            total += (int)irvMillis.Value;
            total += ((int)irvSecs.Value) * 1000;
            total += ((int)irvMins.Value) * 60 * 1000;
            total += ((int)irvHrs.Value) * 3600 * 1000;
            return total;
        }

        private void btnSave_Click(object sender, EventArgs e)
        {
            if (!string.IsNullOrWhiteSpace(txtName.Text))
            {
                string name = txtName.Text;

                // ADD EXCEPTION
                if (name == "DEFAULT")
                {
                    MessageBox.Show(Resources.wrn_cantcreatedef, Resources.wrn_title, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                    return;
                }


                MacroModel macro = new MacroModel
                {
                    Name = name,
                    Description = txtDescription.Text,
                    MouseButton = (MouseButton)cmbMouseButton.SelectedIndex,
                    ClickType = (ClickType)cmbClickType.SelectedIndex,
                    Position = new Position
                    {
                        X = (int)posX.Value,
                        Y = (int)posY.Value,
                        CurrentPosition = true
                    },
                    Interval = GetIntervalInMillis(),
                    RepeatTimes = new RepeatTimes
                    {
                        RepeatForever = repeatForeverBtn.Checked,
                        Count = (int)repeatTimes.Value
                    }
                };
                CreatedMacro = macro;

                MacroManager.SaveMacro(name, macro);
                this.DialogResult = DialogResult.OK;
                this.Close();
            } else
            {
                MessageBox.Show(Resources.err_nullnamecreatemacro, Resources.err_title, MessageBoxButtons.OK, MessageBoxIcon.Error);
            }
        }

        private void ApplyDarkComboBoxStyle(ComboBox comboBox)
        {
            comboBox.DrawMode = DrawMode.OwnerDrawFixed;
            comboBox.FlatStyle = FlatStyle.Flat;
            comboBox.BackColor = Color.FromArgb(40, 40, 40);
            comboBox.ForeColor = Color.White;
            comboBox.DropDownStyle = ComboBoxStyle.DropDownList;

            comboBox.DrawItem += (s, e) =>
            {
                if (e.Index < 0) return;

                ComboBox cmb = (ComboBox)s;
                bool selected = (e.State & DrawItemState.Selected) == DrawItemState.Selected;

                Color backColor = selected ? Color.FromArgb(60, 60, 60) : Color.FromArgb(40, 40, 40);
                Color textColor = Color.White;

                using (Brush backBrush = new SolidBrush(backColor))
                    e.Graphics.FillRectangle(backBrush, e.Bounds);

                using (Brush textBrush = new SolidBrush(textColor))
                    e.Graphics.DrawString(cmb.Items[e.Index].ToString(), e.Font, textBrush, e.Bounds);
            };
        }

        private void MacroCreateForm_Load(object sender, EventArgs e)
        {
            foreach (ComboBox cb in this.Controls.OfType<ComboBox>())
            {
                ApplyDarkComboBoxStyle(cb);
            }
        }
    }
}
