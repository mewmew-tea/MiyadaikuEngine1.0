using System.Threading.Tasks;
using Prism.Mvvm;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using System.Reactive.Disposables;
using System.Windows;
using System;

namespace Miyadaiku.Editor.Core.Controls.Models
{
    public class FloatModel : BindableBase, IDisposable
    {
        protected CompositeDisposable disposables = new CompositeDisposable();
        public ReactivePropertySlim<float> X { get; }

        public FloatModel(float x)
        {
            X = new ReactivePropertySlim<float>(x)
                .AddTo(this.disposables);
        }


        public void Dispose()
        {
            disposables.Dispose();
        }
    }
}
