using System;
using MiyadaikuEngine;

namespace CSScript
{
    [AttributeUsage(AttributeTargets.Property | AttributeTargets.Field,
    AllowMultiple = true,
    Inherited = false)]
    public class SerializeFieldAttribute : Attribute
    {

    }

    public class Game : MonoBehaviour
    {
        [SerializeField]
        float x = 0f;
        [SerializeField]
        float y = 0f;

        [SerializeField]
        Vector3 pos = new Vector3(0,0,0);

        public void Init()
        {
        }
        public void Update()
        {
            // キーボード入力＆移動
            float speed = 0.5f;
            if (GameConsole.GetKey('W'))
            {
                y += speed;
            }
            if (GameConsole.GetKey('S'))
            {
                y -= speed;
            }
            if (GameConsole.GetKey('A'))
            {
                x -= speed;
            }
            if (GameConsole.GetKey('D'))
            {
                x += speed;
            }
            pos.x = x;
            pos.y = y;
            GameConsole.SetPosition(x, y);
        }

        private void ImGuiUpdate()
        {
            if (ImGui.Begin("C# Window"))
            {
                ImGui.Text("This is called from C# script.");
                ImGui.Text($"Pos (Vector3) : {pos.x}, {pos.y}, {pos.z}");
            }
            ImGui.End();
        }
    }
}
