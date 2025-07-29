using Krypton.Toolkit;
using System;
using System.Drawing;
using System.Linq;
using System.Windows.Forms;

public class BaseForm : Form
{
    public virtual void ReloadLanguage() { }

    private Label dummyFocusLabel;

    public BaseForm()
    {
        dummyFocusLabel = new Label
        {
            Size = new Size(1, 1),
            Location = new Point(-10, -10),
            TabStop = false,
            Text = "",
            BackColor = Color.Transparent,
            Visible = true
        };

        Controls.Add(dummyFocusLabel);
        dummyFocusLabel.SendToBack();

        MouseDown += BaseForm_MouseDown;
        Load += (s, e) =>
        {
            dummyFocusLabel.Focus();
            AttachMouseDownToGroupBoxes(this);
            AttachPlaceholderBehavior(this);
        };
    }

    private void BaseForm_MouseDown(object sender, MouseEventArgs e)
    {
        if (GetChildAtPoint(e.Location) == null)
            dummyFocusLabel.Focus();
    }

    private void AttachMouseDownToGroupBoxes(Control parent)
    {
        foreach (var gb in parent.Controls.OfType<GroupBox>())
            gb.MouseDown += GroupBox_MouseDown;

        foreach (Control child in parent.Controls)
            if (child.HasChildren) AttachMouseDownToGroupBoxes(child);
    }

    private void GroupBox_MouseDown(object sender, MouseEventArgs e)
    {
        if (((GroupBox)sender).GetChildAtPoint(e.Location) == null)
            dummyFocusLabel.Focus();
    }

    private void AttachPlaceholderBehavior(Control parent)
    {
        foreach (var ktxt in parent.Controls.OfType<KryptonTextBox>())
            if (ktxt.Tag is string placeholder && !string.IsNullOrWhiteSpace(placeholder))
                SetupPlaceholder(ktxt, placeholder);

        foreach (Control child in parent.Controls)
            if (child.HasChildren) AttachPlaceholderBehavior(child);
    }

    private void SetupPlaceholder(KryptonTextBox textBox, string placeholder)
    {
        textBox.Text = placeholder;
        textBox.StateCommon.Content.Color1 = Color.Gray;

        textBox.Enter += (s, e) =>
        {
            if (textBox.Text == placeholder)
            {
                textBox.Text = "";
                textBox.StateCommon.Content.Color1 = Color.White;
            }
        };

        textBox.Leave += (s, e) =>
        {
            if (string.IsNullOrWhiteSpace(textBox.Text))
            {
                textBox.Text = placeholder;
                textBox.StateCommon.Content.Color1 = Color.Gray;
            }
        };
    }
}
