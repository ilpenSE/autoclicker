using AutoClicker.Properties;
using System;
using System.Collections.Generic;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using static System.Windows.Forms.VisualStyles.VisualStyleElement;

namespace AutoClicker
{
    public partial class MacroForm : Form
    {
        public string SelectedMacroName { get; private set; }

        private string ACTIVE_MACRO_NAME = "DEFAULT";
        private Dictionary<string, MacroModel> _macros;
        private System.Windows.Forms.TextBox editBox = null;
        private ContextMenuStrip contextMenu;

        public MacroForm(Dictionary<string, MacroModel> macros, string activeMacroName)
        {
            InitializeComponent();
            _macros = macros;
            ACTIVE_MACRO_NAME = activeMacroName;
            InitializeContextMenu();
            LoadLanguage();
        }

        private void LoadLanguage()
        {
            createBtn.Text = Resources.create;
            deleteBtn.Text = Resources.delete;
            selectBtn.Text = Resources.select;
            cancelBtn.Text = Resources.cancel;
            label1.Text = Resources.macrolist;
            this.Text = Resources.macroselectiontitle;
        }

        private ToolStripMenuItem createItem;
        private ToolStripMenuItem deleteItem;
        private ToolStripMenuItem renameItem;
        private ToolStripMenuItem editDescItem;

        private void InitializeContextMenu()
        {
            contextMenu = new ContextMenuStrip();

            createItem = new ToolStripMenuItem(MainMenu.CapitalizeFirstLetter(Resources.create), null, (s, e) => createBtn.PerformClick());
            deleteItem = new ToolStripMenuItem(MainMenu.CapitalizeFirstLetter(Resources.delete), null, deleteMacroToolStripMenuItem_Click);
            renameItem = new ToolStripMenuItem(Resources.rename, null, renameMacroToolStripMenuItem_Click);
            editDescItem = new ToolStripMenuItem(Resources.editdesc, null, editDescriptionToolStripMenuItem_Click);

            contextMenu.Items.AddRange(new ToolStripItem[] { createItem, deleteItem, renameItem, editDescItem });

            list.ContextMenuStrip = contextMenu;
            list.MouseClick += list_MouseClick;

            contextMenu.Opening += ContextMenu_Opening;
        }


        private void ContextMenu_Opening(object sender, System.ComponentModel.CancelEventArgs e)
        {
            var mousePos = list.PointToClient(Cursor.Position);
            var hit = list.HitTest(mousePos);

            bool isOnItem = hit.Item != null;

            createItem.Visible = !isOnItem;
            deleteItem.Visible = isOnItem;
            renameItem.Visible = isOnItem;
            editDescItem.Visible = isOnItem;
        }



        private void cancelBtn_Click(object sender, EventArgs e)
        {
            DialogResult = DialogResult.Cancel;
            Close();
        }

        private void saveBtn_Click(object sender, EventArgs e)
        {
            if (list.SelectedItems.Count == 0)
            {
                MessageBox.Show(Resources.info_selectmacro, Resources.info, MessageBoxButtons.OK, MessageBoxIcon.Information);
                return;
            }

            SelectedMacroName = list.SelectedItems[0].Text;
            DialogResult = DialogResult.OK;
        }

        private void LoadMacroList(string selectedMacroName = null)
        {
            list.Items.Clear();
            list.Select();

            foreach (var kvp in _macros)
            {
                var item = new ListViewItem(kvp.Key);
                item.SubItems.Add(kvp.Value.Description ?? "");
                item.Tag = kvp.Key;

                bool isActive = kvp.Key == ACTIVE_MACRO_NAME;
                item.BackColor = isActive ? Color.LightBlue : Color.White;
                item.ForeColor = isActive ? Color.DarkBlue : Color.Black;
                item.Font = new Font(list.Font, isActive ? FontStyle.Bold : FontStyle.Regular);

                list.Items.Add(item);

                if (isActive)
                {
                    item.Selected = true;
                    item.Focused = true;
                    item.EnsureVisible();
                }
            }

        }

        private void createBtn_Click(object sender, EventArgs e)
        {
            using (var createForm = new MacroCreateForm())
            {
                if (createForm.ShowDialog() == DialogResult.OK)
                {
                    _macros.Add(createForm.CreatedMacro.Name, createForm.CreatedMacro);
                    LoadMacroList();
                }
            }
        }

        private void deleteBtn_Click(object sender, EventArgs e) => deleteMacro();

        private void deleteMacroToolStripMenuItem_Click(object sender, EventArgs e) => deleteMacro();

        private void deleteMacro()
        {
            if (list.SelectedItems.Count == 0) return;

            string name = list.SelectedItems[0].Text;
            if (name == "DEFAULT")
            {
                MessageBox.Show(Resources.wrn_cantdeldef, Resources.wrn_title, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            var result = MessageBox.Show(Resources.ask_delmacro.Replace("{selectedName}", name), Resources.ask_confirmdel,
                MessageBoxButtons.YesNo, MessageBoxIcon.Warning);

            if (result == DialogResult.Yes)
            {
                MacroManager.DeleteMacro(name);
                _macros.Remove(name);
                LoadMacroList();
            }
        }

        private void renameMacroToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (list.SelectedItems.Count > 0)
                BeginEdit(list.SelectedItems[0], 0);
        }

        private void editDescriptionToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (list.SelectedItems.Count > 0)
                BeginEdit(list.SelectedItems[0], 1);
        }

        private void BeginEdit(ListViewItem item, int subItemIndex)
        {
            // DEFAULT makrosu düzenlenemez
            if (item.Text == "DEFAULT")
            {
                MessageBox.Show(Resources.wrn_canteditdef, Resources.wrn_title, MessageBoxButtons.OK, MessageBoxIcon.Warning);
                return;
            }

            if (editBox != null) return;

            Rectangle bounds = item.SubItems[subItemIndex].Bounds;

            editBox = new System.Windows.Forms.TextBox
            {
                Bounds = bounds,
                Text = subItemIndex == 0 ? item.Text : item.SubItems[subItemIndex].Text,
                BorderStyle = BorderStyle.FixedSingle
            };

            editBox.LostFocus += (s, e) => EndEdit(item, subItemIndex);
            editBox.KeyDown += (s, e) =>
            {
                if (e.KeyCode == Keys.Enter)
                {
                    e.SuppressKeyPress = true;
                    EndEdit(item, subItemIndex);
                }
                else if (e.KeyCode == Keys.Escape)
                {
                    list.Controls.Remove(editBox);
                    editBox = null;
                }
            };

            item.Tag = item.Text;

            list.Controls.Add(editBox);
            editBox.Focus();
        }

        private void EndEdit(ListViewItem item, int subItemIndex)
        {
            string originalName = item.Tag?.ToString();
            string newText = editBox.Text.Trim();

            list.Controls.Remove(editBox);
            editBox = null;

            if (string.IsNullOrWhiteSpace(newText))
            {
                MessageBox.Show(Resources.err_emptynamedesc, Resources.err_title, MessageBoxButtons.OK, MessageBoxIcon.Error);
                return;
            }

            if (subItemIndex == 0 && item.Text == newText)
                return;
            if (subItemIndex == 1 && item.SubItems[1].Text == newText)
                return;

            if (subItemIndex == 0)
            {
                if (_macros.ContainsKey(newText))
                {
                    MessageBox.Show(Resources.err_macroexists, Resources.err_title, MessageBoxButtons.OK, MessageBoxIcon.Error);
                    return;
                }

                if (originalName != null && _macros.TryGetValue(originalName, out var macro))
                {
                    macro.Name = newText;
                    _macros.Remove(originalName);
                    _macros[newText] = macro;
                    MacroManager.SaveMacro(newText, macro);
                    MacroManager.DeleteMacro(originalName);

                    item.Text = newText;
                    item.Tag = newText;

                    if (originalName == ACTIVE_MACRO_NAME)
                    {
                        ACTIVE_MACRO_NAME = newText;
                        SelectedMacroName = newText;
                    }
                }
            }
            else if (subItemIndex == 1)
            {
                if (originalName != null && _macros.TryGetValue(originalName, out var macro))
                {
                    macro.Description = newText;
                    MacroManager.SaveMacro(originalName, macro);
                    item.SubItems[1].Text = newText;
                }
            }
        }

        private void MacroForm_Load(object sender, EventArgs e)
        {
            list.Columns.Add(Resources.lblname, 200);
            list.Columns.Add(Resources.lbldesc, 282);

            int padding = 1; // 1-2 piksel fazladan boşluk bırak, beyazlık kaybolur
            int totalWidth = list.ClientSize.Width - padding;

            list.Columns[0].Width = 200;
            list.Columns[1].Width = totalWidth - 200 + 1;

            LoadMacroList();
            list.FullRowSelect = true;

            list.KeyDown += list_KeyDown;
            list.MouseDoubleClick += list_MouseDoubleClick;
            list.DrawColumnHeader += list_DrawColumnHeader;
            list.DrawItem += list_DrawItem;
            list.DrawSubItem += list_DrawSubItem;
        }

        private void list_DrawColumnHeader(object sender, DrawListViewColumnHeaderEventArgs e)
        {
            using (SolidBrush backBrush = new SolidBrush(Color.FromArgb(20, 20, 20)))
            using (Pen borderPen = new Pen(Color.FromArgb(60, 60, 60)))
            {
                e.Graphics.FillRectangle(backBrush, e.Bounds);
                e.Graphics.DrawRectangle(borderPen, e.Bounds);
                TextRenderer.DrawText(e.Graphics, e.Header.Text, list.Font, e.Bounds, Color.White);
            }
        }

        private void list_DrawItem(object sender, DrawListViewItemEventArgs e)
        {
            // SubItem'lar ayrı çizileceği için burası boş kalabilir
        }

        private void list_DrawSubItem(object sender, DrawListViewSubItemEventArgs e)
        {
            // Seçili mi kontrol et
            bool selected = e.Item.Selected;

            // 🔧 Renkler
            Color backColor = selected ? Color.FromArgb(54, 173, 169) : Color.FromArgb(20, 20, 20); // Seçili item için özel renk
            Color textColor = selected ? Color.Black : Color.White;

            // Arka planı çiz
            using (SolidBrush backBrush = new SolidBrush(backColor))
                e.Graphics.FillRectangle(backBrush, e.Bounds);

            // Yazıyı çiz
            TextRenderer.DrawText(
                e.Graphics,
                e.SubItem.Text,
                list.Font,
                e.Bounds,
                textColor,
                TextFormatFlags.Left
            );

            // Sağ boşluğu doldur (son sütunsa)
            if (e.ColumnIndex == list.Columns.Count - 1)
            {
                int fullRowWidth = list.ClientSize.Width;
                int usedWidth = 0;
                foreach (ColumnHeader col in list.Columns)
                    usedWidth += col.Width;

                int remainingWidth = fullRowWidth - usedWidth;
                if (remainingWidth > 0)
                {
                    Rectangle fillRect = new Rectangle(e.Bounds.Right, e.Bounds.Top, remainingWidth, e.Bounds.Height);
                    using (Brush brush = new SolidBrush(backColor))
                        e.Graphics.FillRectangle(brush, fillRect);
                }
            }
        }



        private void list_MouseDoubleClick(object sender, MouseEventArgs e)
        {
            var info = list.HitTest(e.Location);
            if (info.Item == null || info.SubItem == null) return;

            int subItemIndex = info.Item.SubItems.IndexOf(info.SubItem);
            BeginEdit(info.Item, subItemIndex);
        }

        private void list_KeyDown(object sender, KeyEventArgs e)
        {
            if (e.KeyCode == Keys.Enter)
            {
                saveBtn_Click(sender, e);
                e.Handled = true;
            } else if (e.KeyCode == Keys.Delete)
            {
                deleteMacro();
                e.Handled = true;
            }
        }

        private void list_MouseClick(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                var hit = list.HitTest(e.Location);
                if (hit.Item != null)
                {
                    list.SelectedItems.Clear();
                    hit.Item.Selected = true;
                    hit.Item.Focused = true;
                }
            }
        }


    }
}