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

namespace Miyadaiku.Editor.MyViews
{
    public class SceneViewHost : HwndHost
    {
        IntPtr app = IntPtr.Zero;

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

        protected override HandleRef BuildWindowCore(HandleRef hwndParent)
        {
            IntPtr hwndHost = CreateWindowEx(
                0, "STATIC", "",
                WindowStyle.WS_CHILD | WindowStyle.WS_VISIBLE,
                0, 0,
                (int)ActualWidth, (int)ActualHeight,
                hwndParent.Handle,
                (IntPtr)WindowStyle.HOST_ID,
                IntPtr.Zero,
                0);

            return new HandleRef(this, hwndHost);
        }

        // TcpClientを使ってホストに対し接続を試みる
        TcpClient client = new TcpClient();
        bool isConnected = false;
        IntPtr hWnd;
        Task updateTask;

        protected override IntPtr WndProc(IntPtr hwnd, int msg, IntPtr wParam, IntPtr lParam, ref bool handled)
        {
            try
            {
                if (!isConnected)
                {
                    IPCManager.Instance.SetUp();
                }
            }
            catch (Exception e)
            {
                handled = false;
                return IntPtr.Zero;
            }

            if (!isConnected)
            {
                isConnected = true;
                hWnd = hwnd;
                updateTask = Task.Run(() =>
                {
                    UpdateFrame();
                });
            }
            //Render(app);
            handled = false;
            return IntPtr.Zero;
        }

        bool isRequestedEndUpdate = false;
        bool isEndUpdate = false;
        void UpdateFrame()
        {
            isRequestedEndUpdate = false;
            isEndUpdate = false;

            while (isRequestedEndUpdate == false)
            {
                SetUpIPCCommand command = new SetUpIPCCommand();
                command.commnadData.hWnd = hWnd.ToInt64();
                command.commnadData.height = (int)ActualHeight;
                command.commnadData.width = (int)ActualWidth;
                IPCManager.Instance.SendAndRecv(command);
            }

            isEndUpdate = true;
        }


        protected override void DestroyWindowCore(HandleRef hwnd)
        {
            isRequestedEndUpdate = true;

            updateTask.Wait();

            DestroyWindow(hwnd.Handle);

            // TcpClientを終了
            if (isConnected)
                client.Close();
            //Dispose(app);
        }

        //====================================
        // Win32 APIs
        //====================================
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
        public ReactiveCollection<ComponentViewModel> Components { get; set; }

        public MainWindowViewModel()
        {
            TestCommand = new DelegateCommand(ExecuteTestCommand);
            TextData = new Dat();
            Components = new ReactiveCollection<ComponentViewModel>();
            Components.Add(new ComponentViewModel(new Models.ComponentModel("AAA")));
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
