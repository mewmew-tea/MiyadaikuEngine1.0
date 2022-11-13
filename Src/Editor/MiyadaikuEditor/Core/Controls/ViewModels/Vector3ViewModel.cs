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

    public class Vector3ViewModel : FieldViewModel/*, IDisposable*/
    {
        public Vector3ViewModel(Models.Vector3Model model)
        {
            this.model = model;
            X = this.model.X
                .ToReactivePropertySlimAsSynchronized(x => x.Value);
            Y = this.model.Y
                .ToReactivePropertySlimAsSynchronized(x => x.Value);
            Z = this.model.Z
                .ToReactivePropertySlimAsSynchronized(x => x.Value);

            ResetCommand = new DelegateCommand(ExecuteTestCommand);
        }

        public Models.Vector3Model model;
        public ReactivePropertySlim<float> X { get; }
        public ReactivePropertySlim<float> Y { get; }
        public ReactivePropertySlim<float> Z { get; }

        //float _textX = 0f;
        float _textY = 0f;
        float _textZ = 0f;
        //public float TextX
        //{ 
        //    get { return _textX; }
        //    set { SetProperty(ref _textX, value); }
        //}
        public float TextY
        {
            get { return _textY; }
            set { SetProperty(ref _textY, value); }
        }
        public float TextZ
        {
            get { return _textZ; }
            set { SetProperty(ref _textZ, value); }
        }


        public DelegateCommand ResetCommand { get; }

        private void ExecuteTestCommand()
        {
            //TextX = 100f;
            TextY = 100f;
            TextZ = 100f;
        }
        //public float X 
        //{
        //    get { return float.Parse(TextX); } 
        //    set { TextX = value.ToString(); } 
        //}
        //public float Y
        //{
        //    get { return float.Parse(TextY); }
        //    set { TextY = value.ToString(); }
        //}
        //public float Z
        //{
        //    get { return float.Parse(TextZ); }
        //    set { TextZ = value.ToString(); }
        //}
    }
}
