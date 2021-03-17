using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Reporting.WinForms;
using System.Data.OleDb;

namespace Rep4
{
    static class ReportHelper
    {
        public static void Viewer<L>(this ReportViewer r, string reportPath, string dataSourceName, IList<L> list, IList<ReportParameter> param)
        {
            var lr = r.LocalReport;
            lr.DataSources.Clear();
            lr.ReportPath = reportPath;
            lr.DataSources.Add(new ReportDataSource(dataSourceName, list));
            lr.SetParameters(param);
            lr.Refresh();
            r.RefreshReport();
        }
       
        public static void Row<T>(this IList<T> l, OleDbDataReader o) where T : new()
        {
            var t = new T();
            foreach (var i in typeof(T).GetProperties()) i.SetValue(t, o[i.Name]);
            l.Add(t);
        }
    }
}
