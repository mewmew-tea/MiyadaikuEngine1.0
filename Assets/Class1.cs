using System;
using System.Text;
using MiyadaikuEngine;

namespace CSScript
{
    public class TestClass
    {
        int num = 0;
        int Num {
            get { Console.WriteLine("Num's \"get\" propaty."); return num; } 
            set { Console.WriteLine("Num's \"set\" propaty."); num = value; } 
        }
        public void PrintAndIncrement()
        {
            Console.WriteLine(Num);
            Num += 1;
        }

    }

    public class MonoBehaviour
    {

    }

    public class CSClass : MonoBehaviour
    {
        private static TestClass testClass = new TestClass();
        private static Game game = new Game();


        [SerializeField]
        float x = 0f;
        [SerializeField]
        float y = 0f;

        public static void Init()
        {
            GameConsole.DebugOutStringLineToVS("Init");
        }

        public static void Update()
        {
            //DefaultTraceListener.Write("aaaa");
            //System.Diagnostics.Debug.Flush();

            game.Update();
            //GameConsole.DebugOutStringLineToVS("aaaaa Omoshire~~~~~!!!");
            //GameConsole.DebugOutStringLineToVS("じゃぱねせは…？日本語は？");

            return;
            PrintMessage();
            Console.WriteLine("=======================================");
            //Encoding e = Encoding.GetEncoding("shift_jis");

            //var sjis = Encoding.GetEncoding("shift_jis");
            //Encoding.RegisterProvider(CodePagesEncodingProvider.Instance);
            Console.WriteLine("************ End of a C# script...(;_;) ************");
            Console.WriteLine("=======================================");
        }

        public static void PrintMessage()
        {
            //String msg = "This is a C# script called from C++!!!!";
            String msg = "This is a C# script called from C++!!!!";
            //String msg = "こんにちは";
            Console.WriteLine(msg);
            Guid guid = Guid.NewGuid();
            //guid;
            Console.WriteLine("************ GUID Test ************");
            Console.WriteLine(guid.ToString());

            Console.WriteLine("************ C# get set(Property) Test ************");
            testClass.PrintAndIncrement();
            testClass.PrintAndIncrement();
            testClass.PrintAndIncrement();
            testClass.PrintAndIncrement();


            Console.WriteLine("************ Call C++ function from C# Test ************");
            GameConsole.PrintMessage();
            GameConsole.SetPosition(20, 100);
            
        }



    }
}
