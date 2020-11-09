using System;
using System.Windows.Forms;
using System.Threading;

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
          //  this.busy
             var t = new Thread(() =>{
                var (list, param) = User.Query();
                BeginInvoke((Action)(() => {
                    reportViewerUser.Viewer(@".\ReportUser.rdlc", "dataSetUser", list, param);
                    reportViewerUser.ShowProgress = false;
                }));
            });
            t.Start();
        }
    }
}
