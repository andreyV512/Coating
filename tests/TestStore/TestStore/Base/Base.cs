using System.Data.OleDb;

namespace BaseX.Base
{
    public class Base
    {
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
