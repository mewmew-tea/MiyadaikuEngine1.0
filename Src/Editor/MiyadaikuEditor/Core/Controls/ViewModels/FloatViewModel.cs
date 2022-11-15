using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Linq;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using System.Reactive.Disposables;


namespace Miyadaiku.Editor.Core.Controls.ViewModels
{
    public class FloatViewModel : FieldViewModel
    {
        public FloatViewModel(Models.FloatModel model)
        {
            this.model = model;
            X = this.model.X
                .ToReactivePropertySlimAsSynchronized(x => x.Value);
        }

        public Models.FloatModel model;
        public ReactivePropertySlim<float> X { get; }
    }
}

