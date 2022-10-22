using System;
using System.Runtime.CompilerServices;

namespace CSScript
{
    internal class GameConsole
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
}
