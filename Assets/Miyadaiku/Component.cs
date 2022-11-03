using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

namespace MiyadaikuEngine
{
    public class Component : Object
    {

        public Transform transform
        {
            get
            {
                return gameObject.transform;
            }
        }

        public GameObject gameObject
        {
            get
            {
                return Internal_GetGameObject(chachedPtr);
            }
        }


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern GameObject Internal_GetGameObject(IntPtr nativeInstance);
    }


    public class MonoBehaviour : Component
    {

    }
}
