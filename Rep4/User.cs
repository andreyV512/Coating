using System.Collections.Generic;
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
        static public (List<UserTable> list, List<ReportParameter> param) Query()
        {
            var list = new List<UserTable>();
            var param = new List<ReportParameter>();
            var b = new Base();
            if (b.Open)
            {
                using (var cmd = b.conn.CreateCommand())
                {
                    cmd.CommandText = "SELECT UserName, UserPersonnelNumber FROM UserTable";
                    var reader = cmd.ExecuteReader();
                    while (reader.Read()) list.Row<UserTable>(reader);
                }
                using (var cmd = b.conn.CreateCommand())
                {
                    cmd.CommandText = "SELECT COUNT(*) FROM UserTable";
                    var count = cmd.ExecuteScalar().ToString();
                    param.Add(new ReportParameter("countUsers", count));
                }
                b.Close();
            }
            else
            {
                MessageBox.Show(
                    "Нет доступа к базе данных. Параметры подключения в файле .\\base.udl"
                    , "Ошибка!"
                    , MessageBoxButtons.OK
                    , MessageBoxIcon.Error
                    );
            }
            return (list, param);
        }
    }
}
