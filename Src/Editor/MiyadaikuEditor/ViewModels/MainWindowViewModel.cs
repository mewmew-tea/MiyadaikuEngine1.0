using Prism.Mvvm;
using Prism.Commands;
using System;
using System.Text;

using System.Threading.Tasks;

using System.Net.Sockets;   // Socket通信をするための名前空間

using System.Windows.Interop;
using System.Runtime.InteropServices;
using Reactive.Bindings;
using System.Configuration;
using Miyadaiku.Editor.Core.IPC;
using Miyadaiku.Editor.Core.IPC.Command;
using System.Text.Json;

namespace Miyadaiku.Editor.MyViews
{
    public class SceneViewHost : HwndHost
    {
        private IntPtr clientHWnd = IntPtr.Zero;
        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            SetUpIPCCommand command = new SetUpIPCCommand();
            command.commnadData.hWnd = hwndParent.Handle.ToInt64();
            command.commnadData.height = (int)ActualHeight;
            command.commnadData.width = (int)ActualWidth;

            var response = JsonSerializer.Deserialize<SetUpIPCCommand.ResponceDataLayout>(IPCManager.Instance.SendAndRecv(command));
            clientHWnd = (IntPtr)response.hWnd;

            return new HandleRef(this, clientHWnd);
        }


        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            clientHWnd = IntPtr.Zero;
        }

        //====================================
        // Win32 APIs
        //====================================
        [Flags]
        enum WindowStyle : int
        {
            WS_CHILD = 0x40000000,
            WS_VISIBLE = 0x10000000,
            LBS_NOTIFY = 0x00000001,
            HOST_ID = 0x00000002,
            LISTBOX_ID = 0x00000001,
            WS_VSCROLL = 0x00200000,
            WS_BORDER = 0x00800000,
        }
        [DllImport("user32.dll")]
        static extern IntPtr CreateWindowEx(int dwExStyle,
                                              string lpszClassName,
                                              string lpszWindowName,
                                              WindowStyle style,
                                              int x, int y,
                                              int width, int height,
                                              IntPtr hwndParent,
                                              IntPtr hMenu,
                                              IntPtr hInst,
                                              [MarshalAs(UnmanagedType.AsAny)] object pvParam);
        [DllImport("user32.dll")]
        static extern bool DestroyWindow(IntPtr hwnd);
    }
}

namespace Miyadaiku.Editor.ViewModels
{

    public class MainWindowViewModel : BindableBase
    {
        private string _title = "Miyadaiku Editor";
        public string Title
        {
            get { return _title; }
            set { SetProperty(ref _title, value); }
        }

        private float testText;
        public float TestText
        {
            get { return testText; }
            set { SetProperty(ref testText, value); }
        }


        public DelegateCommand TestCommand { get; }
        //public ReactiveCollection<ComponentViewModel> Components { get; set; }

        public MainWindowViewModel()
        {
            TestCommand = new DelegateCommand(ExecuteTestCommand);
            TextData = new Dat();
            //Components = new ReactiveCollection<ComponentViewModel>();
            //Components.Add(new ComponentViewModel(new Models.ComponentModel("AAA")));
        }

        public class Dat : BindableBase
        {
            float testText = 0;
            public float TestText
            {
                get { return testText; }
                set { SetProperty(ref testText, value); }
            }
        }
        public Dat TextData;
        //public Dat TextData
        //{
        //    get { return textData; }
        //    set { SetProperty(ref textData, value); }
        //}

        private void ExecuteTestCommand()
        {
            Title = "MUZUIIIIIIIIIIII";
            TextData.TestText = 100;
            //ClientTest(new string[] { "" });
        }

        int ClientTest(string[] args)
        {
            //// TcpClientを使ってホストに対し接続を試みる
            //TcpClient client = new TcpClient();
            //client.Connect("127.0.0.1", 12345);

            //// コマンドラインで入力された文字を取得
            //char[] input = new string("Input!!!").ToCharArray();

            //// 入力された文字をbyteに変換して送信する
            //NetworkStream stream = client.GetStream();
            //byte[] bytes = Encoding.ASCII.GetBytes(input);
            //stream.Write(bytes, 0, bytes.Length);

            //byte[] output = new byte[256];
            //stream.Read(output, 0, output.Length);

            //var ostr = Encoding.ASCII.GetString(output);
            //Console.WriteLine(ostr);

            //Title = ostr;

            //// TcpClientを終了
            //client.Close();
            return 0;
        }

    }

}
