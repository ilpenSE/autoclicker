using System;
using System.Drawing;
using System.Windows.Forms;

public class DarkNumericUpDown : NumericUpDown
{
    private TextBox _textBox;

    public DarkNumericUpDown()
    {
        // Siyah tema renkleri
        this.BackColor = Color.FromArgb(40, 40, 40);
        this.ForeColor = Color.White;
        this.BorderStyle = BorderStyle.FixedSingle;

        this.Controls[0].BackColor = this.BackColor;
        this.Controls[0].ForeColor = this.ForeColor;

        this.Font = new Font("Segoe UI", 9f);

        this.InitializeCustomTextBox();
    }

    private void InitializeCustomTextBox()
    {
        foreach (Control ctrl in this.Controls)
        {
            if (ctrl is TextBox tb)
            {
                _textBox = tb;
                _textBox.BackColor = this.BackColor;
                _textBox.ForeColor = this.ForeColor;
                _textBox.BorderStyle = BorderStyle.None;
                _textBox.ReadOnly = false;
                _textBox.Enabled = true;
                _textBox.TabStop = true;
                _textBox.GotFocus += (s, e) => _textBox.SelectAll();
                break;
            }
        }
    }

    protected override void OnEnter(EventArgs e)
    {
        base.OnEnter(e);
        _textBox?.Focus();
    }

    protected override void OnHandleCreated(EventArgs e)
    {
        base.OnHandleCreated(e);
        InitializeCustomTextBox();
    }
}
