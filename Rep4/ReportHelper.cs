using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Microsoft.Reporting.WinForms;

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
    }
}
