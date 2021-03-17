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

namespace Rep4
{
    public partial class WaitForm : Form
    {
        public WaitForm(Form form)
        {
            InitializeComponent();
            Location = new Point(form.Location.X + (form.Width - Width) / 2
               , form.Location.Y + (form.Height - Height) / 2);
            Show(form);
        }
    }
}
