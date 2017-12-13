using System;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;

namespace Loader
{
    public class NetConnection
    {
        public NetConnection(string hostname = "", int port = 0)
        {
            m_tcpClient = new TcpClient();

            if (hostname != "")
            {
                this.Connect(hostname, port);
            }
        }

        public void Connect(string hostname, int port)
        {
            while (!m_tcpClient.Connected)
            {
                try
                {
                    m_tcpClient.Connect(hostname, port);
                    m_networkStream = m_tcpClient.GetStream();
                }
                catch (Exception)
                {
                    System.Threading.Thread.Sleep(1000);
                }
            }
        }

        public void Disconnect()
        {
            m_tcpClient.Close();
            m_tcpClient = null;
        }

        /// <summary>
        /// Sends an already Prepared Packet to the connected server
        /// </summary>
        /// <param name="packet"></param>
        public void SendPacket(NetPacket packet)
        {
            m_networkStream.Write(packet.ToDataStream(), 0, packet.PacketSize);
        }

        /// <summary>
        /// Recvs a whole Packet from the Server
        /// </summary>
        /// <param name="packet">Needs to point to a valid instance</param>
        public void RecvPacket(NetPacket packet)
        {
            const int iPacketSizeVarInBytes = 4;

            // Keep track of received bytes
            int iOffset = 0;
            // Size in byte form
            byte[] pSize = new byte[iPacketSizeVarInBytes];

            // Recv 4 byte to get size
            do
            {
                int n = m_networkStream.Read(pSize, iOffset, iPacketSizeVarInBytes - iOffset);
                if (n > 0)
                {
                    iOffset += n;
                }
            } while (iOffset < iPacketSizeVarInBytes);

            // Transform size to actual integer
            int iSize = (int)(pSize[0] << 24 |
                              pSize[1] << 16 |
                              pSize[2] << 8 |
                              pSize[3]);

            // Raw data
            byte[] pData = new byte[iSize];
            pSize.CopyTo(pData, 0);

            // Recv whole packet
            do
            {
                int n = m_networkStream.Read(pData, iOffset, iSize - iOffset);
                if (n > 0)
                {
                    iOffset += n;
                }
            } while (iOffset < iSize);

            // Parse raw data to NetPacket
            packet.FromDataStream(pData);
        }

        private TcpClient m_tcpClient;
        private NetworkStream m_networkStream;
    }
}
