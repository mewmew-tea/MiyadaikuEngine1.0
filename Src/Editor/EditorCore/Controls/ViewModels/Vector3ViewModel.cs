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
        }


        string _textX = String.Empty;
        string _textY = String.Empty;
        string _textZ = String.Empty;
        public string TextX
        { 
            get { return _textX; }
            set { SetProperty(ref _textX, value.ToString()); }
        }
        public string TextY
        {
            get { return _textY; }
            set { SetProperty(ref _textY, value.ToString()); }
        }
        public string TextZ
        {
            get { return _textZ; }
            set { SetProperty(ref _textZ, value.ToString()); }
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
