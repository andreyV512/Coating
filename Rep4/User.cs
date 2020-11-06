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
            if (b.Open)
            {
                using (var cmd = b.conn.CreateCommand())
                {
                    cmd.CommandText = "SELECT UserName, UserPersonnelNumber FROM UserTable";
                    var reader = cmd.ExecuteReader();
                    while (reader.Read()) l.Add(Base.Row<UserTable>(reader));
                }
                using (var cmd = b.conn.CreateCommand())
                {
                    cmd.CommandText = "SELECT COUNT(*) FROM UserTable";
                    var count = cmd.ExecuteScalar().ToString();
                    param.Add(new ReportParameter("countUsers", count));
                }
                var lr = viewer.LocalReport;
                lr.DataSources.Clear();
                lr.ReportPath = @".\ReportUser.rdlc";
                lr.DataSources.Add(new ReportDataSource("dataSetUser", l));
                lr.SetParameters(param);
                lr.Refresh();
                viewer.RefreshReport();
                b.Close();
            }
            else
            {
                MessageBox.Show(
                    "Нет доступа к базе данных"
                    , "Ошибка!"
                    , MessageBoxButtons.OK
                    , MessageBoxIcon.Error
                    );
            }
        }
    }
}
