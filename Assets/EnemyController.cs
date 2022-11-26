using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using MiyadaikuEngine;

namespace CSScript
{
    public class EnemyController : MonoBehaviour
    {
        float radius = 0.5f;
        float angle = 0;

        public void Init()
        {
            transform.Position = new Vector3(0,0,5);
        }
        private void Update()
        {
            Vector3 pos = transform.LocalPosition;

            angle += 0.01f;
            pos.x = (float)Math.Cos(angle) * radius;
            pos.y = (float)Math.Sin(angle) * radius;

            transform.LocalPosition = pos;
        }
        private void ImGuiUpdate()
        {

        }

    }
}
