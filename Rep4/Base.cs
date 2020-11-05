using System;
using System.Data.OleDb;
namespace Rep4
{
    public class Base
    {
        public static T Row<T>(OleDbDataReader o) where T : new()
        {
            var t = new T();
            foreach (var i in typeof(T).GetProperties()) i.SetValue(t, o[i.Name]);
            return t;
        }

        public OleDbConnection conn;

        public bool Open()
        {
            try
            {
                conn = new OleDbConnection("File Name=.\\base.udl");
                conn.Open();
                return true;
            }
            catch
            {
                return false;
            }
        }

        public bool IsOpen()
        {
            return null != conn && System.Data.ConnectionState.Open == conn.State;
        }
        public void Close()
        {
            if (IsOpen())
            {
                conn.Close();
            }
        }
    }
}