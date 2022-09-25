using System.Runtime.CompilerServices;
using System.Collections.Generic;

public class UserAssemblyMain
{
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void PrintMessage();

    public static void Awake()
    {

    }

    public static void Init()
    {
        NativeImplement.DebugOutStringLineToVS("Init");

    }

    public static void Update()
    {

    }

    public static void Release()
    {

    }

    private static List<GameObject> gameObjects = new List<GameObject>();
}

public class NativeImplement
{
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void PrintMessage();
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void DebugOutStringToVS(string str);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void DebugOutStringLineToVS(string str);
    //[MethodImplAttribute(MethodImplOptions.InternalCall)]
    //public extern static void PrintMessageWithString();


    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void SetPosition(float _x, float _y);
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool GetKey(char c);
}