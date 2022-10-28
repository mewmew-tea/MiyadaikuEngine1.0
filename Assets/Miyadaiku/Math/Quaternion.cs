using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiyadaikuEngine
{
	public struct Quaternion : IEquatable<Quaternion>, IFormattable
	{
		public float x;
		public float y;
		public float z;
		public float w;

		public float this[int index]
		{
			get
			{
				switch (index)
				{
					case 0:
						return x;
					//break;
					case 1:
						return y;
					//break;
					case 2:
						return z;
					//break;
					case 3:
						return w;
					//break;
					default:
						throw new IndexOutOfRangeException("Quaternion index is out of range!");
				}
			}
			set
			{
				switch (index)
				{
					case 0:
						x = value;
						break;
					case 1:
						y = value;
						break;
					case 2:
						z = value;
						break;
					case 3:
						w = value;
						break;
					default:
						throw new IndexOutOfRangeException("Quaternion index is out of range!");
				}
			}
		}

		public Quaternion(Quaternion q)
        {
			x = q.x; y = q.y; z = q.z; w = q.w;
        }
		public Quaternion(float x, float y, float z, float w)
		{
			this.x = x; this.y = y; this.z = z; this.w = w;
		}

		// Constants
		public static readonly Quaternion Identity = new Quaternion(0f, 0f, 0f, 1f);

		public Quaternion Invert()
		{

			float t = 1.0f / ((x * x) + (y * y) + (z * z) + (w * w));
			return new Quaternion(-x * t, -y * t, -z * t, w * t);
		}
		public Vector3 ToEulerAngles()
		{
			float sx = 2.0f * (-y * z + w * x);

			// ジンバルロック回避
			if (sx < -0.9999f)
			{
				return new Vector3(
					-Mathf.PIDiv2, 0.0f,
					-(float)Math.Atan2(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z))
						);
			}
			if (sx > 0.9999f)
			{
				return new Vector3(
					Mathf.PIDiv2, 0.0f,
					(float)Math.Atan2(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)));
			}

			return new Vector3(
				(float)Math.Asin(sx),
				(float)Math.Atan2(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y)),
				(float)Math.Atan2(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z)));

		}


		public static Quaternion CreateFromYawPitchRoll(float yaw, float pitch,
														float roll)
		{
			return CreateFromEulerAngles(pitch, yaw, roll);
		}

		public static Quaternion CreateFromEulerAngles(float x, float y,
													   float z)
		{
			float halfX = x * 0.5f;
			float sinX = (float)Math.Sin(halfX);
			float cosX = (float)Math.Cos(halfX);

			float halfY = y * 0.5f;
			float sinY = (float)Math.Sin(halfY);
			float cosY = (float)Math.Cos(halfY);

			float halfZ = z * 0.5f;
			float sinZ = (float)Math.Sin(halfZ);
			float cosZ = (float)Math.Cos(halfZ);

			return new Quaternion(
				(cosY * sinX * cosZ) + (sinY * cosX * sinZ),
				(sinY * cosX * cosZ) - (cosY * sinX * sinZ),
				(cosY * cosX * sinZ) - (sinY * sinX * cosZ),
				(cosY * cosX * cosZ) + (sinY * sinX * sinZ));
		}
		public static Quaternion CreateFromEulerAngles(Vector3 angles)
		{
			return CreateFromEulerAngles(angles.x, angles.y, angles.z);
		}



        public override bool Equals(object obj)
        {
            return obj is Quaternion quaternion &&
                   x == quaternion.x &&
                   y == quaternion.y &&
                   z == quaternion.z &&
                   w == quaternion.w;
		}
		public bool Equals(Quaternion other)
		{
			return x == other.x &&
				   y == other.y &&
				   z == other.z &&
				   w == other.w;
		}


		public override int GetHashCode()
        {
            int hashCode = -1743314642;
            hashCode = hashCode * -1521134295 + x.GetHashCode();
            hashCode = hashCode * -1521134295 + y.GetHashCode();
            hashCode = hashCode * -1521134295 + z.GetHashCode();
            hashCode = hashCode * -1521134295 + w.GetHashCode();
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

			return string.Format("({0}, {1}, {2}, {3})", x.ToString(format), y.ToString(format), z.ToString(format), w.ToString(format));
		}
		// 
		public static bool operator ==(Quaternion lhs, Quaternion rhs)
        {
            return (lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w);
        }
        public static bool operator !=(Quaternion lhs, Quaternion rhs)
        {
            return !(lhs == rhs);
        }

		public static Quaternion operator *(Quaternion lhs, Quaternion rhs)
		{
			return new Quaternion(lhs.w * rhs.x + lhs.x * rhs.w + lhs.y * rhs.z - lhs.z * rhs.y,
							  lhs.w * rhs.y - lhs.x * rhs.z + lhs.y * rhs.w + lhs.z * rhs.x,
							  lhs.w * rhs.z + lhs.x * rhs.y - lhs.y * rhs.x + lhs.z * rhs.w,
							  lhs.w * rhs.w - lhs.x * rhs.x - lhs.y * rhs.y - lhs.z * rhs.z);
		}
	}
}
