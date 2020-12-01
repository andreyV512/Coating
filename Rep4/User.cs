using System.Collections.Generic;
using System.Windows.Forms;
using Microsoft.Reporting.WinForms;
using System.Threading.Tasks;

namespace Rep4
{

    public class UserTable
    {
        public string UserName { get; set; }
        public int UserPersonnelNumber { get; set; }
    }

    class User
    {
        static public (List<UserTable>, List<ReportParameter>) Query()
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
                    "Нет доступа к базе данных."
                    , "Ошибка!"
                    , MessageBoxButtons.OK
                    , MessageBoxIcon.Error
                    );
            }
            return (list, param);
        }

        static public async Task<(List<UserTable>, List<ReportParameter>)> QueryAsync()
        {
            var list = new List<UserTable>();
            var param = new List<ReportParameter>();
            var isBaseOpen = false;

            await Task.Run(() =>
            {
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
                    isBaseOpen = true;
                }
            });

            if (!isBaseOpen)
            {
                MessageBox.Show(
                    "Нет доступа к базе данных."
                    , "Ошибка!"
                    , MessageBoxButtons.OK
                    , MessageBoxIcon.Error
                    );
            }
            return (list, param);
        }
    }
}
