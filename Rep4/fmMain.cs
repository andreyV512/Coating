using System;
using System.IO;
using System.Windows.Forms;
using System.Threading;
using System.Windows;
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
            var w = new WaitForm();
            w.Show(this);

            var (list, param) = await User.QueryAsync();
            reportViewerUser.Viewer(@".\ReportUser.rdlc", "dataSetUser", list, param);

            w.Close();
        }

        private async void miSaveClear_Click(object sender, EventArgs e)
        {
            var w = new WaitForm();
            w.Show(this);

            await Task.Run(() => {
                new Base().BackUp();
            });

            miSaveClear.Enabled = false;
            w.Close();
        }

        private async void miOpenBase_Click(object sender, EventArgs e)
        {
            var b = new Base();
            openFileDialog.InitialDirectory = b.backDir;
            openFileDialog.DefaultExt = "bak";
            openFileDialog.Filter = "bak files (*.bak)|*.bak|All files (*.*)|*.*";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                var w = new WaitForm();
                w.Show(this);

                statusLabel1.Text = Path.GetFileName(openFileDialog.FileName);
                miSaveClear.Enabled = false;
                Text = "Архив";

                await Task.Run(() => {
                    b.OpenBase(openFileDialog.FileName);
                });

                w.Close();
            }
            
        }
    }
}
