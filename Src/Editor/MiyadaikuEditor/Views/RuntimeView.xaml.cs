using Miyadaiku.Editor.MyViews;
using System.Windows;
using System.Windows.Controls;

namespace Miyadaiku.Editor.Views
{
    /// <summary>
    /// Interaction logic for RuntimeView
    /// </summary>
    public partial class RuntimeView : UserControl
    {
        private SceneViewHost host = null;
        public RuntimeView()
        {
            InitializeComponent();

            //host = new SceneViewHost();
            //Content = host;
            Loaded += OnRuntimeViewLoaded;
        }
        private void OnRuntimeViewLoaded(object sender, RoutedEventArgs e)
        {
            Loaded -= OnRuntimeViewLoaded;
            Initialize();
        }


        public void Initialize()
        {
            host = new SceneViewHost();
            Content = host;
        }
    }
}
