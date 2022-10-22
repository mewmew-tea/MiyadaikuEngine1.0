using System;

namespace CSScript
{
    internal class Game
    {
        float x = 0f;
        float y = 0f;
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
            GameConsole.SetPosition(x, y);
        }
    }
}
