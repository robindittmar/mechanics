using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Loader
{
    public class NetPacket
    {
        public NetPacket(int iMaxSize = 256)
        {
            m_iMaxSize = iMaxSize;
            this.AllocMemory();
        }

        public int PacketId { get; set; }
        public int PacketSize { get { return m_iPacketSize; } }
        public int StreamPos { get; set; }

        public void FromDataStream(byte[] data)
        {
            int iLenWoutHeader = data.Length - PACKET_HEADER_SIZE;
            if (iLenWoutHeader > m_iMaxSize)
            {
                m_iMaxSize = iLenWoutHeader;
                this.AllocMemory();
            }

            data.CopyTo(m_pData, 0);

            // TODO: Read Header
        }

        public byte[] ToDataStream()
        {
            return m_pData;
        }

        public void WriteByte(byte b)
        {
            m_pData[StreamPos++] = b;
        }

        public void WriteChar(char c)
        {
            m_pData[StreamPos++] = (byte)c;
        }

        public void WriteShort(short s)
        {
            m_pData[StreamPos++] = (byte)((s << 8) & 0xFF);
            m_pData[StreamPos++] = (byte)(s & 0xFF);
        }

        public void WriteInt(int i)
        {
            m_pData[StreamPos++] = (byte)((i << 24) & 0xFF);
            m_pData[StreamPos++] = (byte)((i << 16) & 0xFF);
            m_pData[StreamPos++] = (byte)((i << 8) & 0xFF);
            m_pData[StreamPos++] = (byte)(i & 0xFF);
        }

        public void WriteString(string s)
        {
            byte[] bStr = Encoding.ASCII.GetBytes(s);
            int iLen = bStr.Length;

            this.WriteInt(iLen);
            bStr.CopyTo(m_pData, StreamPos);
            StreamPos += iLen;
        }

        public byte ReadByte()
        {
            return 0;
        }

        public char ReadChar()
        {
            return ' ';
        }

        public short ReadShort()
        {
            return 0;
        }

        public int ReadInt()
        {
            return 0;
        }

        public string ReadString()
        {
            return "";
        }

        public void Prepare()
        {
            this.m_iPacketSize = StreamPos;

            StreamPos = 0;
            this.WriteInt(m_iPacketSize);
            this.WriteInt(PacketId);
            StreamPos = this.m_iPacketSize;
        }

        private void AllocMemory()
        {
            this.StreamPos = PACKET_HEADER_SIZE;
            m_pData = new byte[m_iMaxSize + PACKET_HEADER_SIZE];
        }

        private int m_iPacketSize;

        private int m_iMaxSize;
        private byte[] m_pData;

        public const int PACKET_HEADER_SIZE = 8;
    }
}
