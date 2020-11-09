using System.Data.OleDb;
namespace Rep4
{
    public class Base
    {
        public OleDbConnection conn;

        public bool Open
        {
            get
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
        }

        public bool IsOpen => null != conn && System.Data.ConnectionState.Open == conn.State;

        public void Close()
        {
            if (IsOpen) conn.Close();
        }
    }
}