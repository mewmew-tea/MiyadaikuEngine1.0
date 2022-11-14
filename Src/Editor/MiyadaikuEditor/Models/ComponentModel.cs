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

    public class ComponentModel : BindableBase, IDisposable
    {
        protected CompositeDisposable disposables = new CompositeDisposable();
        public ReactivePropertySlim<string> Name { get; }

        public ComponentModel(string name)
        {
            Name = new ReactivePropertySlim<string>(/*name*/"Transform")
                .AddTo(this.disposables);
        }

        public void Dispose()
        {
            disposables.Dispose();
        }
    }
}
