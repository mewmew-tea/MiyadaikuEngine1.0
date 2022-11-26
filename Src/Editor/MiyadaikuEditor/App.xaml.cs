using Miyadaiku.Editor.Core.IPC;
using Miyadaiku.Editor.Core.IPC.Command;
using Miyadaiku.Editor.Views;
using Prism.Ioc;
using System;
using System.Text.Json;
using System.Collections.Generic;
using System.Windows;
using System.Diagnostics;

namespace Miyadaiku.Editor
{
    /// <summary>
    /// Interaction logic for App.xaml
    /// </summary>
    public partial class App
    {
        private void Application_Startup(object sender, StartupEventArgs e)
        {


            while (true)
            {

                try
                {
                    {
                        IPCManager.Instance.SetUp();
                        break;
                    }
                }
                catch (Exception ex)
                {
                    //handled = false;
                }
            }
            //IPCManager.Instance.SetUp();
        }
        protected override Window CreateShell()
        {
            return Container.Resolve<MainWindow>();
        }

        protected override void RegisterTypes(IContainerRegistry containerRegistry)
        {

        }
    }
}
