using System;
using System.Collections.Generic;
using System.Linq;
using System.Reactive.Disposables;
using System.Text;
using System.Threading.Tasks;
using Prism.Mvvm;
using Reactive.Bindings;
using Reactive.Bindings.Extensions;

using System.Text.Json;
using System.Text.Json.Serialization;
using Miyadaiku.Editor.Core.IPC;
using System.Diagnostics;

namespace Miyadaiku.Editor.Models
{

    public interface IModel
    {

    }

    public class ComponentTypeInfo
    {
        public enum TypeID : UInt32
        {
            // built-in types
            Int = 0,
            Float = 10,
            Bool = 20,
            String = 30,
            IntPtr = 50,
            // MiyadaikuEngine types
            Vector2 = 100,
            Vector3,
            Vector4,
            Quaternion,
            // unsupported types
            Other = UInt16.MaxValue,
        }

        public class FieldTypeInfo
        {
            [JsonPropertyName("name")]
            public string Name { get; set; }

            [JsonPropertyName("typeID")]
            public TypeID TypeID { get; set; }
        }

        [JsonPropertyName("name")]
        public string name { get; set; }

        [JsonPropertyName("nameSpace")]
        public string nameSpace { get; set; }

        [JsonPropertyName("containNonSerialized")]
        public bool containNonSerialized { get; set; }
        
        [JsonPropertyName("fields")]
        public FieldTypeInfo[] fields { get; set; }
    }

    public class FieldModel
    {
        [JsonPropertyName("values")]
        public List<dynamic> Values { get; set; }
    }

    public class ComponentModel : BindableBase, IDisposable
    {
        protected CompositeDisposable disposables = new CompositeDisposable();
        [JsonPropertyName("name")]
        public string name { get { return Name.Value; } set { Name.Value = value; } }
        [JsonIgnore]
        public ReactivePropertySlim<string> Name { get; }

        public ComponentModel()
        {
            Name = new ReactivePropertySlim<string>()
                .AddTo(this.disposables);
        }

        public void Dispose()
        {
            disposables.Dispose();JsonElement j;
        }

        public ComponentTypeInfo TypeInfo { get; set; }

        [JsonPropertyName("values")]
        public List<JsonElement> JsonValues { get; set; }

        public List<dynamic> Values { get; set; }

        public void JsonValuesToDynamic()
        {
            Values = new List<dynamic>();
            foreach (var typeinfo in IPCManager.Instance.TypeInfos)
            {
                if (typeinfo.name == this.name)
                {
                    int i = 0;
                    foreach (var fieldInfo in typeinfo.fields)
                    {
                        var element = JsonValues[i];
                        switch (fieldInfo.TypeID)
                        {
                            case ComponentTypeInfo.TypeID.Int:
                                Values.Add(element.GetInt32());
                                break;
                            case ComponentTypeInfo.TypeID.Float:
                                Values.Add(element.GetDouble());
                                break;
                            case ComponentTypeInfo.TypeID.Bool:
                                Values.Add(element.GetBoolean());
                                break;
                            case ComponentTypeInfo.TypeID.String:
                                Values.Add(new string(element.GetString()));
                                break;
                            case ComponentTypeInfo.TypeID.IntPtr:
                                Values.Add(new IntPtr(element.GetInt64()));
                                break;
                            case ComponentTypeInfo.TypeID.Vector2:
                                Values.Add(new float[2] { (float)(element[0].GetDouble()), (float)(element[1].GetDouble()) });
                                break;
                            case ComponentTypeInfo.TypeID.Vector3:
                                Values.Add(new float[3] { (float)(element[0].GetDouble()), (float)(element[1].GetDouble()), (float)(element[2].GetDouble()) });
                                break;
                            case ComponentTypeInfo.TypeID.Vector4:
                                Values.Add(new float[4] { (float)(element[0].GetDouble()), (float)(element[1].GetDouble()), (float)(element[2].GetDouble()), (float)(element[3].GetDouble()) });
                                break;
                            case ComponentTypeInfo.TypeID.Quaternion:
                                Values.Add(new float[4] { (float)(element[0].GetDouble()), (float)(element[1].GetDouble()), (float)(element[2].GetDouble()), (float)(element[3].GetDouble()) });
                                break;
                            case ComponentTypeInfo.TypeID.Other:
                                Values.Add(0);
                                break;
                            default:
                                Values.Add(0);
                                break;
                        }
                        ++i;
                    }
                }
            }
        }
    }
}
