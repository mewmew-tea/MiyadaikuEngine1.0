using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace MiyadaikuEngine
{
    public class EffekseerEmitter : Component
    {        
        public int Play()
        {
            return Internal_Play(transform.Position.x, transform.Position.y, transform.Position.z);
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern int Internal_Play(float x, float y, float z);
    }
}
