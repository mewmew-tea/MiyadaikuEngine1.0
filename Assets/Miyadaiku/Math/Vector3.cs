using System;

namespace MiyadaikuEngine
{
    public struct Vector3 : IEquatable<Vector3>, IFormattable
    {
        public float x;
        public float y;
        public float z;

        public Vector3(Vector3 v)
        {
            this.x = v.x; this.y = v.y; this.z = v.z;
        }
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

        /// <summary>
        /// normalized vector. (read only)
        /// </summary>
        public Vector3 normalized
        {
            get
            {
                return Normalize(this);
            }
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

        public static Vector3 operator +(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
        }

        public static Vector3 operator -(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
        }

        public static Vector3 operator -(Vector3 a)
        {
            return new Vector3(0f - a.x, 0f - a.y, 0f - a.z);
        }

        public static Vector3 operator *(Vector3 a, float b)
        {
            return new Vector3(a.x * b, a.y * b, a.z * b);
        }

        public static Vector3 operator *(float b, Vector3 a)
        {
            return new Vector3(a.x * b, a.y * b, a.z * b);
        }

        public static Vector3 operator /(Vector3 a, float b)
        {
            return new Vector3(a.x / b, a.y / b, a.z / b);
        }

        public static Vector3 Normalize(Vector3 value)
        {
            Vector3 result = new Vector3(value);
            result.Normalize();
            return result;
        }
        public void Normalize()
        {
            float l = length;
            if (l == 0.0f)
            {
                return;
            }
            l = 1.0f / l;
            x *= l;
            y *= l;
            z *= l;
        }

        public static float Dot(Vector3 lhs, Vector3 rhs)
        {
            return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
        }
        public static Vector3 Cross(Vector3 lhs, Vector3 rhs)
        {
            return new Vector3((lhs.y * rhs.z) - (lhs.z * rhs.y), (lhs.z * rhs.x) - (lhs.x * rhs.z),
                           (lhs.x * rhs.y) - (lhs.y * rhs.x));
        }

        public static Vector3 Lerp(Vector3 start, Vector3 end, float t)
        {
            return new Vector3(Mathf.Lerp(start.x, end.x, t),
                               Mathf.Lerp(start.y, end.y, t),
                               Mathf.Lerp(start.z, end.z, t));
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

        public override string ToString()
        {
            return ToString(null, null);

        }

        public string ToString(string format)
        {
            return ToString(format, null);

        }

        public string ToString(string format, IFormatProvider formatProvider)
        {
            if (string.IsNullOrEmpty(format))
            {
                format = "F5";
            }

            return string.Format("({0}, {1}, {2})", x.ToString(format), y.ToString(format), z.ToString(format));

        }
    }
}

