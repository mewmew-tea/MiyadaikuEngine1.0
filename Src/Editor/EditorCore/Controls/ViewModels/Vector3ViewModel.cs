using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Miyadaiku.EditorCore.Controls.ViewModels
{
    public class Vector3ViewModel : BindableBase
    {
        public Vector3ViewModel()
        {
            //X = 0f;
            //Y = 0f;
            //Z = 0f;

            ResetCommand = new DelegateCommand(ExecuteTestCommand);
        }
        public string Name { get; set; } = "YYYYYYYYYYYY";


        float _textX = 0f;
        float _textY = 0f;
        float _textZ = 0f;
        public float TextX
        { 
            get { return _textX; }
            set { SetProperty(ref _textX, value); }
        }
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
            TextX = 100f;
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
