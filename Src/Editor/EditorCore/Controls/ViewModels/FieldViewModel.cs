using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Linq;

namespace EditorCore.Controls.ViewModels
{
    public class FieldViewModel : BindableBase
    {
        public string Name { get; private set; }
        public string Type { get; private set; }
        public FieldViewModel()
        {
        }
    }
}
