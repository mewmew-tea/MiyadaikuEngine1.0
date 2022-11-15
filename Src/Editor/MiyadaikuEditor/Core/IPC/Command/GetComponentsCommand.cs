using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Text.Json;
using System.Text.Json.Serialization;
using Miyadaiku.Editor.Models;

namespace Miyadaiku.Editor.Core.IPC.Command
{
    internal class GetComponentsCommand : CommandBase
    {
        public override string ToJson()
        {
            return JsonSerializer.Serialize(commnadData);
        }

        public class CommandDataLayout
        {
            [JsonPropertyName("commandID")]
            public string commandID { get { return "GetComponents"; } }
        }

        public class ResponseDataLayout
        {
            [JsonPropertyName("components")]
            public ComponentModel[] Components { get; set; }
            //[JsonPropertyName("typeInfos")]
            //public ComponentTypeInfo[] TypeInfos { get; set; }
        }


        public CommandDataLayout commnadData = new CommandDataLayout();
        public ResponseDataLayout responseData = new ResponseDataLayout();
    }
}
