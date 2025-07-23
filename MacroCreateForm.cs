using AutoClicker.Properties;
using Krypton.Toolkit;
using System;
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
            cmodBtn.DataSource = new string[]
            {
                Resources.opt_cm_click,
                Resources.opt_cm_hold
            };

            LoadLanguageValues();
            txtName.Select();
        }

        private void LoadLanguageValues()
        {
            lblName.Text = Resources.lblname + ":";
            lblDesc.Text = Resources.lbldesc + ":";
            lblMouse.Text = Resources.opt_msbtn + ":";
            lblClick.Text = Resources.opt_clicktype + ":";
            lblMod.Text = Resources.opt_cmod + ":";
            intervallbl.Text = Resources.interval_groupbox + ":";
            holddurlbl.Text = Resources.holddur_groupbox + ":";
            millislbl.Text = Resources.interval_millis;
            hd_millislbl.Text = Resources.interval_millis;
            irvInfo.Text = Resources.interval_info;
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
            return (int)irvMillis.Value;
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

                if (MacroManager.Macros.ContainsKey(name))
                {
                    MessageBox.Show(Resources.err_macroexists, Resources.err_title, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }


                MacroModel macro = new MacroModel
                {
                    Name = name,
                    Description = string.IsNullOrWhiteSpace(txtDescription.Text) ? Resources.nodesc : txtDescription.Text,
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
                    },
                    ClickMode = (ClickMod)cmodBtn.SelectedIndex,
                    HoldDuration = (int)hdMillis.Value
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

        private void MacroCreateForm_Load(object sender, EventArgs e)
        {
            this.Font = FontLoader.GetRegular(9f);
            btnSave.StateCommon.Content.ShortText.Font = MainMenu.ButtonFont;

            RegisterTextBoxFocusEvents();
            RegisterNumericFocusEvents();
            RegisterDropdownFocusEvents();
        }
        private void RegisterTextBoxFocusEvents()
        {
            foreach (KryptonTextBox txtbox in new KryptonTextBox[] { txtDescription, txtName })
            {
                txtbox.MouseEnter += (s, ev) =>
                {
                    if (!txtbox.Focused)
                        txtbox.StateCommon.Border.Color1 = MainMenu.focusBorderClr;
                };

                txtbox.MouseLeave += (s, ev) =>
                {
                    if (!txtbox.Focused)
                        txtbox.StateCommon.Border.Color1 = MainMenu.normalBorderClr;
                };

                txtbox.GotFocus += (s, ev) =>
                {
                    txtbox.StateCommon.Border.Color1 = MainMenu.focusBorderClr;
                };

                txtbox.LostFocus += (s, ev) =>
                {
                    txtbox.StateCommon.Border.Color1 = MainMenu.normalBorderClr;
                };
            }
        }

        private void RegisterNumericFocusEvents()
        {
            foreach (KryptonNumericUpDown kryptonNumericUpDown in new KryptonNumericUpDown[] { irvMillis, hdMillis, repeatTimes, posX, posY })
            {
                kryptonNumericUpDown.MouseEnter += (s, ev) =>
                {
                    if (!kryptonNumericUpDown.Focused)
                        kryptonNumericUpDown.StateCommon.Border.Color1 = MainMenu.focusBorderClr;
                };

                kryptonNumericUpDown.MouseLeave += (s, ev) =>
                {
                    if (!kryptonNumericUpDown.Focused)
                        kryptonNumericUpDown.StateCommon.Border.Color1 = MainMenu.normalBorderClr;
                };

                kryptonNumericUpDown.GotFocus += (s, ev) =>
                {
                    kryptonNumericUpDown.StateCommon.Border.Color1 = MainMenu.focusBorderClr;
                };

                kryptonNumericUpDown.LostFocus += (s, ev) =>
                {
                    kryptonNumericUpDown.StateCommon.Border.Color1 = MainMenu.normalBorderClr;
                };
            }
        }

        private void RegisterDropdownFocusEvents()
        {
            foreach (KryptonComboBox box in new KryptonComboBox[] { cmodBtn, cmbClickType, cmbMouseButton })
            {
                box.MouseEnter += (s, ev) =>
                {
                    box.StateActive.ComboBox.Border.Color1 = MainMenu.focusBorderClr;
                };

                box.MouseLeave += (s, ev) =>
                {
                    box.StateActive.ComboBox.Border.Color1 = MainMenu.normalBorderClr;
                };

                box.GotFocus += (s, ev) =>
                {
                    box.StateActive.ComboBox.Border.Color1 = MainMenu.focusBorderClr;
                };

                box.LostFocus += (s, ev) =>
                {
                    box.StateActive.ComboBox.Border.Color1 = MainMenu.normalBorderClr;
                };
            }
        }
    }
}
