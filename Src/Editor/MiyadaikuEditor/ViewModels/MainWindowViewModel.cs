using Prism.Mvvm;

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
