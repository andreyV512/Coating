using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using Microsoft.Reporting.WinForms;

namespace Rep4
{
    public class UserTable
    {
        public string UserName { get; set; }
        public int UserPersonnelNumber { get; set; }
    }

    class User
    {
        static public void Query(ReportViewer viewer)
        {
            var l = new List<UserTable>();
            var param = new List<ReportParameter>();
            var b = new Base();
            if (b.Open())
            {
                {
                    OleDbCommand cmd = b.conn.CreateCommand();
                    cmd.CommandText = "SELECT UserName, UserPersonnelNumber FROM UserTable";
                    OleDbDataReader reader = cmd.ExecuteReader();
                    while (reader.Read()) l.Add(Base.Row<UserTable>(reader));
                }
                {
                    OleDbCommand cmd = b.conn.CreateCommand();
                    cmd.CommandText = "SELECT COUNT(*) FROM UserTable";
                    var count = cmd.ExecuteScalar().ToString();
                    param.Add(new ReportParameter("countUsers", count));
                    
                }
                viewer.LocalReport.DataSources.Clear();
                viewer.LocalReport.ReportPath = @".\ReportUser.rdlc";
                var dataset = new ReportDataSource("dataSetUser", l);
                viewer.LocalReport.DataSources.Add(dataset);
                viewer.LocalReport.SetParameters(param);
                viewer.LocalReport.Refresh();
                viewer.RefreshReport();
                b.Close();
            }
        }
    }
}
