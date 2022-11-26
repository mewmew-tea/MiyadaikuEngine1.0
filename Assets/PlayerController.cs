using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MiyadaikuEngine;

namespace CSScript
{
    public class PlayerController : MonoBehaviour
    {
        public float speed = 0.05f;
        public int interval = 40;
        private int counter = 0;
        List<GameObject> bullets;
        List<GameObject> enemies;
        private void Init()
        {
            // find bullet gameobjects
            GameObject[] gameObjects = gameObject.GetAll();
            bullets = new List<GameObject>();
            foreach (GameObject obj in gameObjects)
            {
                if (obj.GetComponent<BulletController>() != null)
                {
                    bullets.Add(obj);
                }
            }

            enemies = new List<GameObject>();
            foreach (GameObject obj in gameObjects)
            {
                if (obj.GetComponent<EnemyController>() != null)
                {
                    enemies.Add(obj);
                }
            }
            //bullets = Array.FindAll(gameObjects, g => g.GetComponent<BulletController>() != null);
        }

        private void Update()
        {
            Vector3 pos;
            pos = transform.LocalPosition;

            // キーボード入力＆移動
            if (Runtime.GetKey('W'))
            {
                pos.y += speed;
            }
            if (Runtime.GetKey('S'))
            {
                pos.y -= speed;
            }
            if (Runtime.GetKey('A'))
            {
                pos.x -= speed;
            }
            if (Runtime.GetKey('D'))
            {
                pos.x += speed;
            }

            // Shot
            if (Runtime.GetKey('P'))
            {
                Shot();
            }
            ++counter;


            // Shot
            if (Runtime.GetKey('R'))
            {
                foreach (GameObject enemy in enemies)
                {
                    if (!enemy.Enabled)
                    {
                        enemy.Enabled = true;
                    }
                }
            }

            transform.LocalPosition = pos;
        }
        private void ImGuiUpdate()
        {
            if (ImGui.Begin("Player"))
            {
                ImGui.Text($"Counter : {counter}");
                ImGui.Text($"Bullets : {bullets.Count()}");
            }
            ImGui.End();

        }


        private void Shot()
        {            
            if (counter < interval)
            {
                return;
            }
            foreach (GameObject bullet in bullets)
            {
                BulletController controller = (BulletController)bullet.GetComponent<BulletController>();

                if (controller == null)
                {
                    continue;
                }

                if (!bullet.Enabled)
                {
                    controller.Shot(transform.Position);
                    counter = 0;
                    return;
                }
            }
        }

    }
}
