using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiyadaikuEngine
{
    public class Mathf
    {
        // Constants

        // PI
        public static readonly float PI = 3.141592654f;
        // PI * 2
        public static readonly float PI2 = 6.283185307f;
        // PI / 2
        public static readonly float PIDiv2 = 0.159154943f;


        // Math functions
        public static float Lerp(float a, float b, float t)
        {
            return a * (1 - t) + b * t;
        }
    }
}
