using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace Rep4
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            this.reportViewer1.RefreshReport();
            User.Query(reportViewer1);
        }

        private void reportViewer1_Click(object sender, EventArgs e)
        {
            this.Text = "klick";
        }

        private void reportViewer1_Load(object sender, EventArgs e)
        {

        }

        private void reportViewer1_Enter(object sender, EventArgs e)
        {
            this.Text = "enter";
            User.Query(reportViewer1);
        }
    }
}
