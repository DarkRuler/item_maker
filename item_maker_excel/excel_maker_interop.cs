using System;
using System.Runtime.InteropServices;
using Excel = Microsoft.Office.Interop.Excel;
using Microsoft.Vbe.Interop;

namespace item_maker_excel
{
    public class excel_maker_interop
    {
        public static void make_excel_file(String csv_filename)
        {
            Excel.Application excel = new Excel.Application();
            excel.Visible = true;
            excel.DisplayAlerts = false;            

            Excel.Workbook workbook = excel.Workbooks.Add();
            Excel.Worksheet worksheet = workbook.Worksheets["Sheet1"];

            VBProject project = workbook.VBProject;
            VBComponent component = project.VBComponents.Add(vbext_ComponentType.vbext_ct_StdModule);           

            component.CodeModule.AddFromString(String.Format(Properties.Resources.items_info_macro, csv_filename));
            excel.Run("items_info_macro");
            project.VBComponents.Remove(component);
           
            String xlsx_filename = utility.show_save_dialog("Excel Workbook (*.xlsx)|*.xlsx|All Files (*.*)|*.*", 1);           
            if (xlsx_filename != "") workbook.SaveAs(xlsx_filename);

            workbook.Close();
            excel.Quit();

            Marshal.ReleaseComObject(workbook);
            Marshal.ReleaseComObject(excel);
        }        
    }
}
