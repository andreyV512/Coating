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
                var res = User.Query();
                this.BeginInvoke((Action)(() => {
                    reportViewerUser.AddData(@".\ReportUser.rdlc", "dataSetUser", res.list, res.param);
                }));
            });
            t.Start();
        }
    }
}
