using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Miyadaiku.Editor.Core.Controls.ViewModels
{
    public class FieldViewModel : BindableBase
    {
        public string Name { get;  set; }
        public string Type { get;  set; }
        public FieldViewModel()
        {
        }
    }
}
