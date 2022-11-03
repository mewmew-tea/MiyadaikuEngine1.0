using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiyadaikuEngine
{
    [AttributeUsage(AttributeTargets.Property | AttributeTargets.Field,
    AllowMultiple = true,
    Inherited = false)]
    public class SerializeFieldAttribute : Attribute
    {

    }
}
