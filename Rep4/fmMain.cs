using System;
using System.Windows.Forms;
using System.Threading;
using Microsoft.Reporting.WinForms;
//using ReportHelper;
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
            var t = new Thread(() =>{
                var (list, param) = User.Query();
                this.BeginInvoke((Action)(() => {
                    reportViewerUser.Viewer(@".\ReportUser.rdlc", "dataSetUser", list, param);
                }));
            });
            t.Start();
        }
    }
}
