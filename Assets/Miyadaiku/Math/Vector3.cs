using System;

namespace MiyadaikuEngine
{
    public struct Vector3 : IEquatable<Vector3>
    {
        public float x;
        public float y;
        float z;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public Vector3(float x, float y)
        {
            this.x = x;
            this.y = y;
            z = 0f;
        }

        /// <summary>
        /// Length of this vector. (read only)
        /// </summary>
        public float length
        {
            get { return (float)Math.Sqrt(x * x + y * y + z * z); }
        }
        /// <summary>
        /// Squared length of this vector. (read only)
        /// </summary>
        public float lengthSquared
        {
            get { return x * x + y * y + z * z; }
        }


        //=============================
        
        public static bool operator ==(Vector3 lhs, Vector3 rhs)
        {
            return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z);
        }

        public static bool operator !=(Vector3 lhs, Vector3 rhs)
        {
            return !(lhs == rhs);
        }

        public override bool Equals(object obj)
        {
            return obj is Vector3 vector &&
                   x == vector.x &&
                   y == vector.y &&
                   z == vector.z;
        }
        public bool Equals(Vector3 other)
        {
            return x == other.x && y == other.y && z == other.z;
        }

        public override int GetHashCode()
        {
            int hashCode = 373119288;
            hashCode = hashCode * -1521134295 + x.GetHashCode();
            hashCode = hashCode * -1521134295 + y.GetHashCode();
            hashCode = hashCode * -1521134295 + z.GetHashCode();
            return hashCode;
        }
    }
}

