using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Text.Json;
using System.Text.Json.Serialization;
using System.Threading.Tasks;

namespace Miyadaiku.Editor.Core.IPC.Command
{
    internal class SetUpIPCCommand : CommandBase
    {

        public override string ToJson()
        {
            return JsonSerializer.Serialize(commnadData);
        }

        public class CommandDataLayout
        {
            [JsonPropertyName("commandID")]
            public string commandID { get { return "SetUpIPC"; } }

            [JsonPropertyName("hWnd")]
            public long hWnd { get; set; }
            
            [JsonPropertyName("width")]
            public int width { get; set; }
            
            [JsonPropertyName("height")]
            public int height { get; set; }

        }

        public class ResponceDataLayout
        {

            [JsonPropertyName("result")]
            public bool result { get; set; } = false;
            [JsonPropertyName("hWnd")]
            public long hWnd { get; set; }
        }


        public CommandDataLayout commnadData = new CommandDataLayout();
    }
}
