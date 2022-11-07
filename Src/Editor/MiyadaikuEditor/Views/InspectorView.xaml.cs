using System.Windows.Controls;
using System.Windows;

using System.Windows.Controls.Primitives;
using System.Windows.Data;

using Prism.Mvvm;

namespace Miyadaiku.Editor.Views
{
    /// <summary>
    /// Interaction logic for InspectorView
    /// </summary>
    public partial class InspectorView : UserControl
    {
        public InspectorView()
        {
            InitializeComponent();
        }
        private void ToggleButton_Loaded(object sender, RoutedEventArgs e)
        {
            var btn = (ToggleButton)sender;

            btn.SetBinding(ToggleButton.IsCheckedProperty, new Binding("IsOpen") { Source = btn.ContextMenu });
            btn.ContextMenu.PlacementTarget = btn;
            btn.ContextMenu.Placement = PlacementMode.Bottom;
        }
    }
}
