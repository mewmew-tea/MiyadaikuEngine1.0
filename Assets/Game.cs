using System;
using MiyadaikuEngine;

namespace CSScript
{
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
        private void Update()
        {
            // キーボード入力＆移動
            float speed = 0.05f;
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

            transform.LocalPosition = pos;
        }

        private void ImGuiUpdate()
        {
            if (ImGui.Begin("C# Window"))
            {
                ImGui.Text("This is called from C# script.");
                ImGui.Text($"Pos (Vector3) : " + pos);
                ImGui.Text($"chachePtr : " + chachedPtr);
                ImGui.Text($"transform.LocalPosition : " + transform.LocalPosition);
            }
            ImGui.End();
        }
    }
    public class Game2 : MonoBehaviour
    {[SerializeField]
        Vector3 pos = new Vector3(0, 0, 0);

        float angle = 0;

        public void Init()
        {
        }
        private void Update()
        {
            float speed = 0.5f;
            float radius = 0.5f;

            angle += 0.01f;
            pos.x = (float)Math.Cos(angle) * radius;
            pos.y = (float)Math.Sin(angle) * radius;

            transform.LocalPosition = pos;
        }

        private void ImGuiUpdate()
        {
            if (ImGui.Begin("C# Window 02"))
            {
                ImGui.Text("This is called from C# script.");
                ImGui.Text($"Pos (Vector3) : " + pos);
                ImGui.Text($"chachePtr : " + chachedPtr);
                ImGui.Text($"transform.LocalPosition : " + transform.LocalPosition);
            }
            ImGui.End();
        }
    }
}
