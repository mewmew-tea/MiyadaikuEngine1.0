using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MiyadaikuEngine;

namespace CSScript
{
    public class BulletController : MonoBehaviour
    {
        public float speed = 0.5f;
        List<GameObject> enemies;
        
        private void Init()
        {
            // find enemy gameobjects
            GameObject[] gameObjects = gameObject.GetAll();

            enemies = new List<GameObject>();
            foreach (GameObject obj in gameObjects)
            {
                if (obj.GetComponent<EnemyController>() != null)
                {
                    enemies.Add(obj);
                }
            }
            //enemies = Array.FindAll(gameObjects, g => g.GetComponent<EnemyController>() != null);
        }

        private void Update()
        {
            if (transform.Position.z >= 10)
            {
                Destroy();
            }

            foreach (GameObject enemy in enemies)
            {
                if (!enemy.Enabled)
                {
                    continue;
                }

                float distance = (float)Math.Sqrt(
                    (transform.Position.x - enemy.transform.Position.x) * (transform.Position.x - enemy.transform.Position.x) +
                    + (transform.Position.y - enemy.transform.Position.y) * (transform.Position.y - enemy.transform.Position.y)
                    + (transform.Position.z - enemy.transform.Position.z) * (transform.Position.z - enemy.transform.Position.z));
                if (distance < 0.5f)
                {
                    enemy.Enabled = false;
                    var emitter = (EffekseerEmitter)gameObject.GetComponent<EffekseerEmitter>();
                    if (emitter != null)
                    {
                        emitter.Play();
                    }
                    Destroy();
                }
            }
            transform.Position += new Vector3(0f, 0f, speed);
        }
        private void ImGuiUpdate()
        {

        }

        public void Shot(Vector3 pos)
        {
            gameObject.Enabled = true;
            transform.LocalPosition = pos;
        }

        public void Destroy()
        {
            gameObject.Enabled = false;
        }
    }
}
