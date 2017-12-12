using System;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Loader
{
    public partial class Login : Form
    {
        TcpClient m_tcpClient;
        public Login()
        {
            InitializeComponent();

            m_tcpClient = new TcpClient("wpy.ddns.net", 50504);
        }

        private void btnLogin_Click(object sender, System.EventArgs e)
        {
            NetPacket loginPacket = new NetPacket();
            loginPacket.PacketId = 100;
            loginPacket.WriteString(txtId.Text);
            loginPacket.WriteString(txtPw.Text);
            loginPacket.Prepare();

            m_tcpClient.GetStream().Write(loginPacket.ToDataStream(), 0, loginPacket.PacketSize);

            // TODO: Receive response & check wether it was ok or not
            if (txtId.Text == "admin" &&
                txtPw.Text == "admin")
            {
                m_tcpClient.Close();

                DialogResult = DialogResult.OK;
            }
        }
    }
}
