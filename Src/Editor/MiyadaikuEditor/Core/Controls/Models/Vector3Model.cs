using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Prism.Mvvm;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using System.Reactive.Disposables;
using System.Windows;

namespace Miyadaiku.Editor.Core.Controls.Models
{

    public class Vector3Model : BindableBase, IDisposable
    {
        protected CompositeDisposable disposables = new CompositeDisposable();
        public ReactivePropertySlim<float> X { get; }
        public ReactivePropertySlim<float> Y { get; }
        public ReactivePropertySlim<float> Z { get; }

        public Vector3Model(float x, float y, float z)
        {
            X = new ReactivePropertySlim<float>(x)
                .AddTo(this.disposables);
            Y = new ReactivePropertySlim<float>(y)
                .AddTo(this.disposables);
            Z = new ReactivePropertySlim<float>(z)
                .AddTo(this.disposables);
        }


        public void Dispose()
        {
            disposables.Dispose();
        }
    }
}
