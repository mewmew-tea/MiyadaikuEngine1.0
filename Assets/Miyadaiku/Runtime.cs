using System;
using System.Runtime.CompilerServices;

namespace MiyadaikuEngine
{
    public class Runtime
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void PrintMessage();
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void DebugOutStringToVS(string str);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void DebugOutStringLineToVS(string str);
        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //public extern static void PrintMessageWithString();


        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void SetPosition(float _x, float _y);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool GetKey(char c);
    }

    public class ImGui
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool Begin(string label);
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void End();
        public static void Text(string format, params object[] ary)
        {
            var text = String.Format(format, ary);
            Text_Internal(text);
        }
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Text_Internal(string text);
    }

    public class GuidProvider
    {
        public static string NewGuid()
        {
            return Guid.NewGuid().ToString("N");
        }

        
    }

}
