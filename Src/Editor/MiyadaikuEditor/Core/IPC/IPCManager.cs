using System;
using System.Collections.Generic;
using System.Configuration;
using System.Diagnostics;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using Miyadaiku.Editor.Core.IPC.Command;

namespace Miyadaiku.Editor.Core.IPC
{
    class IPCManager
    {
        //------------------------------
        // Singleton
        //------------------------------
        private static IPCManager instance;

        public static IPCManager Instance
        {
            get
            {
                if (instance == null)
                    instance = new IPCManager();

                return instance;
            }
        }
        private IPCManager()
        {

        }

        
        TcpClient tcpClient;
        NetworkStream networkStream;

        public bool isConnected
        {
            get
            {
                if (tcpClient == null || tcpClient.Client == null)
                {
                    return false;
                }
                return tcpClient.Connected;
            }
        }

        public void SetUp()
        {
            var hostname = ConfigurationManager.AppSettings.Get("RuntimeHost");
            var port = int.Parse(ConfigurationManager.AppSettings.Get("RuntimePort"));

            tcpClient = new TcpClient();
            tcpClient.Connect(hostname, port);
            networkStream = tcpClient.GetStream();
        }

        public string SendAndRecv(CommandBase command)
        {
            var commandData = System.Text.Encoding.ASCII.GetBytes(command.ToJson());
            byte[] commandLength = BitConverter.GetBytes(commandData.Length);
            networkStream.Write(commandLength);
            //WaitForResponse();
            networkStream.Write(commandData);
            return WaitForResponse();
        }


        private string WaitForResponse()
        {
            byte[] responseLengthData = new byte[4];
            byte[] data;
            bool isCompleted = false;

            try
            {
                while (isCompleted == false)
                {
                    networkStream.Read(responseLengthData, 0, 4);
                    int responseLength = BitConverter.ToInt32(responseLengthData);

                    data = new byte[responseLength];
                    int bytesRead = networkStream.Read(data, 0, Math.Min(2048, responseLength));
                    return Encoding.ASCII.GetString(data, 0, bytesRead);
                }
            }
            catch (Exception)
            {
                //Debug.Assert(messageLength >= 0);
            }

            return String.Empty;
        }

        void TickRuntimeProcess()
        {
            // 
        }
    }
}
