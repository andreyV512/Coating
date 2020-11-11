using System;
using System.IO;
using System.Windows.Forms;
using System.Threading;
using System.Windows;

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

        private void miOperators_Click(object sender, EventArgs e)
        {
            reportViewerUser.ShowProgress = true;
            var w = new WaitForm();
            w.Show(this);
            var t = new Thread(() =>{
                var (list, param) = User.Query();
                BeginInvoke((Action)(() => {
                    reportViewerUser.Viewer(@".\ReportUser.rdlc", "dataSetUser", list, param);
                    reportViewerUser.ShowProgress = false;
                    w.Close();
                }));
            });
            t.Start();
        }

        private void miSaveClear_Click(object sender, EventArgs e)
        {
            var b = new Base();
            b.BackUp();
            miSaveClear.Enabled = false;
        }

        private void miOpenBase_Click(object sender, EventArgs e)
        {
            var b = new Base();
            openFileDialog.InitialDirectory = b.backDir;
            openFileDialog.DefaultExt = "bak";
            openFileDialog.Filter = "bak files (*.bak)|*.bak|All files (*.*)|*.*";
            if (openFileDialog.ShowDialog() == DialogResult.OK)
            {
                var w = new WaitForm();
                w.Show(this);
                var t = new Thread(() => {
                    var (list, param) = User.Query();
                    BeginInvoke((Action)(() => {
                        b.OpenBase(openFileDialog.FileName);
                        statusLabel1.Text = Path.GetFileName(openFileDialog.FileName);
                        miSaveClear.Enabled = false;
                        w.Close();
                    }));
                });
                t.Start();
            }
            
        }

        private void reportViewerUser_Load(object sender, EventArgs e)
        {
           
        }
    }
}
