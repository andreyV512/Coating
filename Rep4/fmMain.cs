using System;
using System.IO;
using System.Windows.Forms;
using System.Threading.Tasks;

namespace Rep4
{
    public partial class fmUser : Form
    {
        public fmUser()
        {
            InitializeComponent();
        }

        private void miExit_Click(object sender, EventArgs e)
        {
            Close();
        }

        private async void miOperators_Click(object sender, EventArgs e)
        {
            using (new WaitForm(this))
            {
                var (list, param) = await User.QueryAsync();
                reportViewerUser.Viewer(@".\ReportUser.rdlc", "dataSetUser", list, param);
            }
        }

        private async void miSaveClear_Click(object sender, EventArgs e)
        {
            using (new WaitForm(this))
            {
                await Task.Run(() =>{new Base().BackUp();});
                miSaveClear.Enabled = false;
            }
        }

        private async void miOpenBase_Click(object sender, EventArgs e)
        {
            var b = new Base();
            openFileDialog.InitialDirectory = b.backDir;
            openFileDialog.DefaultExt = "bak";
            openFileDialog.Filter = "bak files (*.bak)|*.bak|All files (*.*)|*.*";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                using (new WaitForm(this))
                {
                    statusLabel1.Text = Path.GetFileName(openFileDialog.FileName);
                    miSaveClear.Enabled = false;
                    Text = "Архив";

                    await Task.Run(() =>{b.OpenBase(openFileDialog.FileName);});
                }
            }
        }

        private void miOpenCurrentBaseToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Base.ConnectionStringOpenCurrentBase();
            statusLabel1.Text = "";
            Text = "База";
            reportViewerUser.Clear();
        }
}
}
