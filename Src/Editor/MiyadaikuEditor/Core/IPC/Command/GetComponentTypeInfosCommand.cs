using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;
using Miyadaiku.Editor.Models;

namespace Miyadaiku.Editor.Core.IPC.Command
{
    internal class GetComponentTypeInfosCommand : CommandBase
    {

        public override string ToJson()
        {
            return JsonSerializer.Serialize(commnadData);
        }

        public class CommandDataLayout
        {
            [JsonPropertyName("commandID")]
            public string commandID { get { return "GetComponentTypeInfos"; } }
        }

        public class ResponseDataLeyout
        {
            [JsonPropertyName("typeInfos")]
            public ComponentTypeInfo[] TypeInfos { get; set; }
        }


        public CommandDataLayout commnadData = new CommandDataLayout();
    }
}
