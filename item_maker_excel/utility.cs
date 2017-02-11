using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace item_maker_excel
{
    public class utility
    {
        public static String show_save_dialog(String filter, int index)
        {
            var file_dialog = new System.Windows.Forms.SaveFileDialog();

            file_dialog.Filter = filter;
            file_dialog.FilterIndex = index;
            file_dialog.RestoreDirectory = true;

            if (file_dialog.ShowDialog() == System.Windows.Forms.DialogResult.OK)
            {
                return file_dialog.FileName;
            }
            else return "";
        }
    }
}
