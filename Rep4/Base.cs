using System.Data.OleDb;
using System;
using System.IO;
using System.Data;
using System.Linq.Expressions;
using System.Threading;

namespace Rep4
{
    public class Base
    {
        static string connectionString = @"Provider = SQLOLEDB.1; Integrated Security = SSPI; 
                            Persist Security Info = False; Initial Catalog = StoreBase;
                            Data Source = (local)";

        public OleDbConnection conn;

        public bool Open
        {
            get
            {
                try
                {
                    conn = new OleDbConnection(connectionString);
                    conn.Open();
                    return true;
                }
                catch
                {
                    return false;
                }
            }
        }

        public string backDir
        {
            get
            {
                return Path.GetDirectoryName(Environment.GetCommandLineArgs()[0]) + "\\BACK";
            }
        }

        public bool IsOpen => null != conn && ConnectionState.Open == conn.State;

        public void Close()
        {
            if (IsOpen) conn.Close();
        }

        public void BackUp()
        {
            if(Open)
            {
                Directory.CreateDirectory(backDir);

                DateTime d = DateTime.Now;

                var nameBackFile = String.Format("{0,2}_{1,2}_{2,2}_{3,2}{4,2}{5,2}"
                    , d.Year
                    , d.Month
                    , d.Day
                    , d.Hour
                    , d.Minute
                    , d.Second
                    );

                
                using (var cmd = conn.CreateCommand())
                {
                    cmd.CommandText = String.Format(@"BACKUP DATABASE StoreBase 
                        TO DISK='{0}\{1}.bak' WITH FORMAT
                        , MEDIANAME='SQLServerBackups'
                        , NAME='Full Backup of StoreBase'"
                    , backDir, nameBackFile
                    ); 
                    cmd.ExecuteNonQuery();
                }

                using (var cmd = conn.CreateCommand())
                {
                    cmd.CommandText = "sp_MSForEachTable 'TRUNCATE TABLE ?'";
                    cmd.ExecuteNonQuery();
                }
                Close();
            }
        }

        public void OpenBase(string fileName)
        {
            connectionString = @"Provider = SQLOLEDB.1; Integrated Security = SSPI; 
                            Persist Security Info = False; Initial Catalog = StoreBase;
                            Data Source = (local)";
            if (Open)
            {
                using (var cmd = conn.CreateCommand())
                {
                    cmd.CommandText = "OpenBase";
                    cmd.CommandType = CommandType.StoredProcedure;
                    cmd.Parameters.Add(new OleDbParameter("@path", backDir));
                    cmd.Parameters.Add(new OleDbParameter("@file_name", fileName));

                    cmd.ExecuteNonQuery();

                }

                connectionString = @"Provider = SQLOLEDB.1; Integrated Security = SSPI; 
                            Persist Security Info = False; Initial Catalog = StoreBaseTMP;
                            Data Source = (local)";

                Close(); 
            }
        }
    }
}