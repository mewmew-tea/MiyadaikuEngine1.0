using Prism.Commands;
using System.Collections.ObjectModel;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Linq;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;

namespace Miyadaiku.Editor.ViewModels
{
    public class InspectorViewModel : BindableBase
    {
        public ReactiveCollection<ComponentViewModel> Components { get; set; }

        public string targetObjectName = "";
        public string TargetObjectName
        {
            get { return targetObjectName; }
            set { SetProperty(ref targetObjectName, value); }
        }
        public InspectorViewModel()
        {
            TargetObjectName = "GameObject";
            Components = new ReactiveCollection<ComponentViewModel>();
            Models.ComponentModel model = new Models.ComponentModel("Transform");
            //model.Name.Value = "Transform";
            var comp = new ComponentViewModel(model/*"Transform"*/);
            ////comp.model.Name.Value = "Transform";
            ////comp.Name = "Transform";
            Components.Add(comp);

            AddPlayerControllerCommand = new DelegateCommand(AddPlayerController);
        }

        public DelegateCommand AddPlayerControllerCommand { get; }

        void AddPlayerController()
        {
            //Models.ComponentModel model = new Models.ComponentModel("Transform");
            //var comp = new ComponentViewModel(model/*"Transform"*/);
            //model.Name.Value = "Transform";


            ////comp.Name = "PlayerController";
            //Components.Add(comp);
        }

    }
}
