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

        public MainWindowViewModel()
        {
        }
    }

}
