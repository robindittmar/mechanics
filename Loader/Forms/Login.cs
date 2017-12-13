using System;
using System.Net.Sockets;
using System.Runtime.InteropServices;
using System.Windows.Forms;

namespace Loader
{
    public partial class Login : Form
    {
        NetConnection m_connection;
        public Login()
        {
            InitializeComponent();

            m_connection = new NetConnection("wpy.ddns.net", 50504);
        }

        private void btnLogin_Click(object sender, System.EventArgs e)
        {
            NetPacket loginPacket = new NetPacket
            {
                PacketId = NetPacket.PacketIdLogin
            };
            loginPacket.WriteInt((int)(DateTime.UtcNow.Subtract(new DateTime(1970, 1, 1))).TotalSeconds); // TODO: WriteLong (8-byte, ASAP)
            loginPacket.WriteString(txtId.Text);
            loginPacket.WriteString(txtPw.Text);
            loginPacket.Prepare();
            m_connection.SendPacket(loginPacket);

            NetPacket ackPacket = new NetPacket();
            m_connection.RecvPacket(ackPacket);
            
            if (ackPacket.PacketId == NetPacket.PacketIdOk)
            {
                // Login successful
                DialogResult = DialogResult.OK;
            }
            else
            {
                int errorCode = ackPacket.ReadInt(); // unused, but need to read so we can access the error msg
                MessageBox.Show($"Error: {ackPacket.ReadString()}");
            }
            
            /*if (txtId.Text == "admin" &&
                txtPw.Text == "admin")
            {
                m_connection.Disconnect();

                DialogResult = DialogResult.OK;
            }*/
        }
    }
}
