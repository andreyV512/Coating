using System;
using System.Data.OleDb;


namespace TestStore
{
    class Program
    {
        public class UserTable
        {
            public string UserName;// { get; set; }
            public int UserPersonnelNumber;// { get; set; }
        }
        static void TestUserTable()
        {
            var b = new BaseX.Base.Base();
            if (b.Open())
            {
                Console.WriteLine("Base open");
                string[] m = { "Ifanoff", "Petroff", "Sidoroff", "Xrenoff" };
                int k = 5;
                try
                {
                    foreach (var i in m)
                    {
                        OleDbCommand cmd = b.conn.CreateCommand();
                        cmd.CommandText = "INSERT INTO UserTable(UserName, UserPersonnelNumber)VALUES(?, ?)";
                        cmd.Parameters.Add(new OleDbParameter("", OleDbType.VarChar) { Value = i });
                        cmd.Parameters.Add(new OleDbParameter("", OleDbType.Integer) { Value = k++ });
                        cmd.ExecuteNonQuery();
                    }
                }
                catch (System.Data.OleDb.OleDbException)
                {
                    Console.WriteLine("Err");
                }
            }
        }

        static void Conv<T>(ref T t, Object o) => t = (T)o;

        static void TestGetUser()
        {
            var u = new UserTable();
            var b = new BaseX.Base.Base();
            if (b.Open())
            {
                OleDbCommand cmd = b.conn.CreateCommand();
                cmd.CommandText = "SELECT UserName, UserPersonnelNumber FROM UserTable";
                OleDbDataReader reader = cmd.ExecuteReader();
                while(reader.Read())
                {
                    Conv(ref u.UserName, reader[0]);
                    Conv(ref u.UserPersonnelNumber, reader[1]);
                    Console.WriteLine("{0}  {1}", u.UserName, u.UserPersonnelNumber);
                }
            }
        }

        static void Main(string[] args)
        {
            //TestUserTable();
            // UserTable ut;
            TestGetUser();
            Console.WriteLine("Hello World!");
        }
    }
}
