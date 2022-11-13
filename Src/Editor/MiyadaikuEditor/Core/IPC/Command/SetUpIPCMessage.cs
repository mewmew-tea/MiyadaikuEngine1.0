﻿using System;
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
        public override string MessageType { get { return "SetUpSystemMessage"; } }

        public override string ToJson()
        {
            return JsonSerializer.Serialize(commnadData);
        }

        public class CommandDataLayout
        {
            [JsonPropertyName("hWnd")]
            public long hWnd { get; set; }
            
            [JsonPropertyName("width")]
            public int width { get; set; }
            
            [JsonPropertyName("height")]
            public int height { get; set; }

        }

        public class ResponceDataLeyout
        {
            public bool result { get; set; } = false;
        }


        public CommandDataLayout commnadData = new CommandDataLayout();
    }
}
