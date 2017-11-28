using System;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Loader
{
    public partial class Login : Form
    {
        public Login()
        {
            InitializeComponent();
        }

        private void btnLogin_Click(object sender, System.EventArgs e)
        {
            if (txtId.Text == "admin" &&
                txtPw.Text == "admin")
            {
                DialogResult = DialogResult.OK;
            }
        }
    }
}
