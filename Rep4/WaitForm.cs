using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Windows.Forms;
using System.Windows.Interop;
using System.Threading;

namespace Rep4
{
    public partial class WaitForm : Form
    {
        private const int GWL_STYLE = -16; //WPF's Message code for Title Bar's Style 
        private const int WS_SYSMENU = 0x80000; //WPF's Message code for System Menu

        private Thread t;

        [DllImport("user32.dll", SetLastError = true)]
        private static extern int GetWindowLong(IntPtr hWnd, int nIndex);
        [DllImport("user32.dll")]
        private static extern int SetWindowLong(IntPtr hWnd, int nIndex, int dwNewLong);
        public WaitForm()
        {
            InitializeComponent();
        }

        private void WaitForm_Load(object sender, EventArgs e)
        {
            Location = new System.Drawing.Point(Owner.Location.X + (Owner.Width - Width) / 2, Owner.Location.Y + (Owner.Height - Height) / 2);
            SetWindowLong(this.Handle, GWL_STYLE, GetWindowLong(this.Handle, 0));
            t = new Thread(() => {
                int val = 0;
                int dv = 10;
                while (true)
                {
                    BeginInvoke((Action)(() =>
                    {
                        val += dv;
                        if (val > progressBar.Maximum - 20) dv = -10;
                        else if (val < progressBar.Minimum + 20) dv = 10;
                        progressBar.Value = val;
                    }));
                    Thread.Sleep(10);
                }
            });
            t.Start();
        }

        private void WaitForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            t.Abort();
        }
    }
}
