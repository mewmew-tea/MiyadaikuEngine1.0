using Prism.Commands;
using Prism.Mvvm;
using System;
using System.Collections.Generic;
using System.Collections.ObjectModel;
using System.Linq;
using Miyadaiku.Editor.Core.Controls.ViewModels;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;
using System.Reactive.Disposables;
using Miyadaiku.Editor.Core.Controls.Models;
using Miyadaiku.Editor.Models;
using System.Timers;

namespace Miyadaiku.Editor.ViewModels
{
    public class ComponentViewModel : BindableBase, IDisposable
    {
        protected CompositeDisposable disposables = new CompositeDisposable();
        public Models.ComponentModel model;

        public ReactivePropertySlim<string> Name { get; }
        public ObservableCollection<FieldViewModel> Fields { get; set; }

        public string AddComponentBorderName = "";

        public ComponentViewModel(Models.ComponentModel model)
        {
            //this.model.Name.Value = "Transform";
            this.model = model;
            this.model.AddTo(this.disposables);
            Name = this.model.Name
                // ViewとModelの値を双方向で紐付ける
                .ToReactivePropertySlimAsSynchronized(x => x.Value)
                .AddTo(this.disposables);
            Fields = new ObservableCollection<FieldViewModel>();

            for (int i = 0; i < model.TypeInfo.fields.Length; ++i)
            {
                var fieldInfo = model.TypeInfo.fields[i];
                switch (fieldInfo.TypeID)
                {
                    case ComponentTypeInfo.TypeID.Float:
                        {
                            FloatModel valueModel = new FloatModel(model.Values[i]);
                            var field = new FloatViewModel(valueModel);
                            field.Name = fieldInfo.Name;
                            Fields.Add(field);
                        }
                        break;
                    case ComponentTypeInfo.TypeID.Vector3:
                        {
                            Vector3Model valueModel = new Vector3Model(model.Values[i][0], model.Values[i][1], model.Values[i][2]);
                            var field = new Vector3ViewModel(valueModel);
                            field.Name = fieldInfo.Name;
                            Fields.Add(field);
                        }
                        break;
                    case ComponentTypeInfo.TypeID.Int:
                        break;
                    case ComponentTypeInfo.TypeID.Bool:
                        break;
                    case ComponentTypeInfo.TypeID.String:
                        break;
                    case ComponentTypeInfo.TypeID.IntPtr:
                        break;
                    case ComponentTypeInfo.TypeID.Vector2:
                        break;
                    case ComponentTypeInfo.TypeID.Vector4:
                        break;
                    case ComponentTypeInfo.TypeID.Quaternion:
                        break;
                    case ComponentTypeInfo.TypeID.Other:
                        break;
                    default:
                        break;
                }
            }
        }

        public void UpdateFields()
        {

            for (int i = 0; i < model.TypeInfo.fields.Length; ++i)
            {
                var fieldInfo = model.TypeInfo.fields[i];

                var fields = Fields.Where(x=>x.Name == fieldInfo.Name);
                if (fields.Any() == false)
                {
                    continue;
                }
                var field = fields.First();

                switch (fieldInfo.TypeID)
                {
                    case ComponentTypeInfo.TypeID.Float:
                        {
                            var vm = field as FloatViewModel;
                            vm.X.Value = model.Values[i];
                        }
                        break;
                    case ComponentTypeInfo.TypeID.Vector3:
                        {
                            var vm = field as Vector3ViewModel;
                            vm.X.Value = model.Values[i][0];
                            vm.Y.Value = model.Values[i][1];
                            vm.Z.Value = model.Values[i][2];
                        }
                        break;
                    case ComponentTypeInfo.TypeID.Int:
                        break;
                    case ComponentTypeInfo.TypeID.Bool:
                        break;
                    case ComponentTypeInfo.TypeID.String:
                        break;
                    case ComponentTypeInfo.TypeID.IntPtr:
                        break;
                    case ComponentTypeInfo.TypeID.Vector2:
                        break;
                    case ComponentTypeInfo.TypeID.Vector4:
                        break;
                    case ComponentTypeInfo.TypeID.Quaternion:
                        break;
                    case ComponentTypeInfo.TypeID.Other:
                        break;
                    default:
                        break;
                }
            }
        }
        public void Dispose()
        {
            disposables.Dispose();
        }
    }
}
