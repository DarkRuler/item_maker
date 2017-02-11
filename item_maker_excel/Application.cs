using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace item_maker_excel
{
    public class application
    {
        [STAThread]
        static void Main(string[] parameters)
        {
            String csv_filename = utility.show_save_dialog("Comma-Separated Values (*.csv)|*.csv|All Files (*.*)|*.*", 1);
            excel_maker_interop.make_excel_file(csv_filename);
        }
    }
}
