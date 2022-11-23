using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

namespace MiyadaikuEngine
{
    public class GameObject : Object
    {

        public Transform transform
        {
            get
            {
                return Internal_GetTransform(chachedPtr);
            }
        }

        public Component[] GetComponents()
        {
            return Internal_GetComponents(chachedPtr);
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Transform Internal_GetTransform(IntPtr nativeInstance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Component[] Internal_GetComponents(IntPtr nativeInstance);

    }
}
