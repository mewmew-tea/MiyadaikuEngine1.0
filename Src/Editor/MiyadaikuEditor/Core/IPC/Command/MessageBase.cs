using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Miyadaiku.Editor.Core.IPC.Command
{
    abstract class CommandBase
    {
        public abstract string MessageType { get; }

        public abstract string ToJson();

    }
}
