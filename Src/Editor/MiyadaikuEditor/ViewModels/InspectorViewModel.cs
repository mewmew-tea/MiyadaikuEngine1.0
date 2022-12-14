using Prism.Commands;
using System.Collections.ObjectModel;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Configuration;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using Miyadaiku.Editor.Core.IPC.Command;
using System.Text.Json;
using Miyadaiku.Editor.Core.IPC;
using System.Timers;

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
            Models.ComponentModel model = new Models.ComponentModel();

            // TODO: multi components

            // Get type infos
            {
                GetComponentTypeInfosCommand command = new GetComponentTypeInfosCommand();

                var response = JsonSerializer.Deserialize<GetComponentTypeInfosCommand.ResponseDataLeyout>(IPCManager.Instance.SendAndRecv(command));

                foreach (var info in response.TypeInfos)
                {
                    if (info.name == "Transform")
                    {
                        model.TypeInfo = info;
                        model.Name.Value = info.name;
                        break;
                    }
                }
            }
            // Get components
            {

                GetComponentsCommand command = new GetComponentsCommand();

                var response = JsonSerializer.Deserialize<GetComponentsCommand.ResponseDataLayout>(IPCManager.Instance.SendAndRecv(command));

                model.JsonValues = response.Components[0].JsonValues;
                model.JsonValuesToDynamic();
            }

            // Add component
            var comp = new ComponentViewModel(model);
            Components.Add(comp);

            // Update by timer
            Timer timer = new Timer(500);
            timer.Elapsed += (sender, e) =>
            {
                timer.Stop();
                GetComponentsCommand command = new GetComponentsCommand();

                var response = JsonSerializer.Deserialize<GetComponentsCommand.ResponseDataLayout>(IPCManager.Instance.SendAndRecv(command));

                model.JsonValues = response.Components[0].JsonValues;
                model.JsonValuesToDynamic();
                Components[0].model.Values = model.Values;
                Components[0].UpdateFields();

                timer.Start();
            };
            timer.Start();

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
