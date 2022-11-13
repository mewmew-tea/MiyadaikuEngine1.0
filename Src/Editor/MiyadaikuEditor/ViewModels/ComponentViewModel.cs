using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using Miyadaiku.Editor.Core.Controls.ViewModels;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using System.Reactive.Disposables;
using Miyadaiku.Editor.Core.Controls.Models;

namespace Miyadaiku.Editor.Models
{
    public interface IModel
    {

    }

    public class ComponentModel : BindableBase, IDisposable
    {
        protected CompositeDisposable disposables = new CompositeDisposable();
        public ReactivePropertySlim<string> Name { get; }

        public ComponentModel(string name)
        {
            Name = new ReactivePropertySlim<string>(/*name*/"Transform")
                .AddTo(this.disposables);
        }

        public void Dispose()
        {
            disposables.Dispose();
        }
    }
}
namespace Miyadaiku.Editor.ViewModels
{
    public class ComponentViewModel : BindableBase, IDisposable
    {
        protected CompositeDisposable disposables = new CompositeDisposable();
        //private string _name = "";
        //public string Name
        //{
        //    get { return _name; }
        //    set { SetProperty(ref _name, value); }
        //}

        //public ReactiveProperty<string> Text { get; }
        public Models.ComponentModel model;
        //public ReactiveProperty<string> Name { get; set; }

        public ReactivePropertySlim<string> Name { get; }
        public ObservableCollection<Vector3ViewModel> Fields { get; set; }

        public string AddComponentBorderName = "";

        public ComponentViewModel(Models.ComponentModel model)
        {
            //this.model.Name.Value = "Transform";
            model = new Models.ComponentModel("AAAAAA");
            this.model = model;
            this.model.AddTo(this.disposables);
            Name = this.model.Name
                // ViewとModelの値を双方向で紐付ける
                .ToReactivePropertySlimAsSynchronized(x => x.Value)
                .AddTo(this.disposables);
            //Name = new string(name);
            //Name = "TTTTTTTTTTTT";
            Fields = new ObservableCollection<Vector3ViewModel>();
            {
                Vector3Model valueModel = new Vector3Model(0,0,0);
                var field = new Vector3ViewModel(valueModel);
                field.Name = "Position";
                Fields.Add(field);
            }
            {
                Vector3Model valueModel = new Vector3Model(0, 0, 0);
                var field = new Vector3ViewModel(valueModel);
                field.Name = "Rotation";
                Fields.Add(field);
            }
            {
                Vector3Model valueModel = new Vector3Model(1, 1, 1);
                var field = new Vector3ViewModel(valueModel);
                field.Name = "Scale";
                Fields.Add(field);
            }
        }
        public void Dispose()
        {
            disposables.Dispose();
        }
    }
}
