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

        public Component GetComponent<T>()
        {
            var components = Internal_GetComponents(chachedPtr);
            foreach (var component in components)
            {
                if (component.GetType() == typeof(T))
                {
                    return component;
                }
            }
            return null;
        }
        public GameObject[] GetAll()
        {
            return Internal_GetAll(chachedPtr);
        }

        public bool Enabled
        {
            get 
            { 
                return Internal_GetEnabled(chachedPtr);
            }
            set 
            { 
                Internal_SetEnabled(chachedPtr, value); 
            }
        }

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Transform Internal_GetTransform(IntPtr nativeInstance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern Component[] Internal_GetComponents(IntPtr nativeInstance);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern GameObject[] Internal_GetAll(IntPtr nativeInstance);


        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern bool Internal_GetEnabled(IntPtr _chachedPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void Internal_SetEnabled(IntPtr _chachedPtr, bool _enabled);

    }
}
