using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace item_maker_excel
{
    public class utility
    {
        public static String show_open_dialog(String filter, int index)
        {
            var open_dialog = new System.Windows.Forms.OpenFileDialog();

            open_dialog.Filter = filter;
            open_dialog.FilterIndex = index;
            open_dialog.RestoreDirectory = true;

            if (open_dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                return open_dialog.FileName;
            }
            else return "";
        }

        public static String show_save_dialog(String filter, int index)
        {
            var save_dialog = new System.Windows.Forms.SaveFileDialog();

            save_dialog.Filter = filter;
            save_dialog.FilterIndex = index;
            save_dialog.RestoreDirectory = true;

            if (save_dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                return save_dialog.FileName;
            }
            else return "";
        }
    }
}
