using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace MiyadaikuEngine
{
    /// <summary>
    /// Matrix(4x4) struct(Row-major). 
    /// Rotation order is "ZXY".
    /// </summary>
    public struct Matrix : IEquatable<Matrix>, IFormattable
    {
        //--------------------------
        // Matrix data(row-major)
        //--------------------------
		public float m11;
        public float m12;
		public float m13;
		public float m14;
		public float m21;
		public float m22;
		public float m23;
		public float m24;
		public float m31;
		public float m32;
		public float m33;
		public float m34;
		public float m41;
		public float m42;
		public float m43;
		public float m44;

        public readonly static Matrix Identity = new Matrix(1f,0f,0f,0f,
                                                            0f,1f,0f,0f,
                                                            0f,0f,1f,0f,
                                                            0f,0f,0f,1f);

        /// <summary>
        /// Get as array.( float[16] { m11, m12, m13, m14, m21, m22, m23... } )
        /// </summary>
        /// <returns>continuous data(array)</returns>
        public float[] GetAsArray()
        {
            return new float[16] { m11, m12, m13, m14, m21, m22, m23, m24, m31, m32, m33, m34, m41, m42, m43, m44 };
        }

		
        public float this[int row, int column]
        {
            get
            {
                return this[row * 4 + column];
            }
            set
            {
                this[row * 4 + column] = value;
            }
        }

        public float this[int index]
        {
            get
            {
                switch (index)
                {
                    case 0:
                        return m11;
                        //break;
                    case 1:
                        return m12;
                        //break;
                    case 2:
                        return m13;
                        //break;
                    case 3:
                        return m14;
                        //break;
                    case 4:
                        return m21;
                        //break;
                    case 5:
                        return m22;
                        //break;
                    case 6:
                        return m23;
                        //break;
                    case 7:
                        return m24;
                        //break;
                    case 8:
                        return m31;
                        //break;
                    case 9:
                        return m32;
                        //break;
                    case 10:
                        return m33;
                        //break;
                    case 11:
                        return m34;
                        //break;
                    case 12:
                        return m41;
                        //break;
                    case 13:
                        return m42;
                        //break;
                    case 14:
                        return m43;
                        //break;
                    case 15:
                        return m44;
                        //break;
                    default:
                        throw new IndexOutOfRangeException("Matrix index is out of range!");
                }
            }
            set
            {
                switch (index)
                {
                    case 0:
                        m11 = value;
                        break;
                    case 1:
                        m12 = value;
                        break;
                    case 2:
                        m13 = value;
                        break;
                    case 3:
                        m14 = value;
                        break;
                    case 4:
                        m21 = value;
                        break;
                    case 5:
                        m22 = value;
                        break;
                    case 6:
                        m23 = value;
                        break;
                    case 7:
                        m24 = value;
                        break;
                    case 8:
                        m31 = value;
                        break;
                    case 9:
                        m32 = value;
                        break;
                    case 10:
                        m33 = value;
                        break;
                    case 11:
                        m34 = value;
                        break;
                    case 12:
                        m41 = value;
                        break;
                    case 13:
                        m42 = value;
                        break;
                    case 14:
                        m43 = value;
                        break;
                    case 15:
                        m44 = value;
                        break;
                    default:
                        throw new IndexOutOfRangeException("Matrix index is out of range!");
                }
            }
        }

        public Matrix(Matrix src)
        {
            this.m11 = src.m11; this.m12 = src.m12; this.m13 = src.m13; this.m14 = src.m14;
            this.m21 = src.m21; this.m22 = src.m22; this.m23 = src.m23; this.m24 = src.m24;
            this.m31 = src.m31; this.m32 = src.m32; this.m33 = src.m33; this.m34 = src.m34;
            this.m41 = src.m41; this.m42 = src.m42; this.m43 = src.m43; this.m44 = src.m44;
        }

        public Matrix(float m11, float m12, float m13, float m14, float m21,
               float m22, float m23, float m24, float m31, float m32,
               float m33, float m34, float m41, float m42, float m43,
               float m44)
        {
            this.m11 = m11; this.m12 = m12; this.m13 = m13; this.m14 = m14;
            this.m21 = m21; this.m22 = m22; this.m23 = m23; this.m24 = m24;
            this.m31 = m31; this.m32 = m32; this.m33 = m33; this.m34 = m34;
            this.m41 = m41; this.m42 = m42; this.m43 = m43; this.m44 = m44;
        }


        public static Matrix CreateTranslation(float x, float y, float z)
        {
            return new Matrix(1.0f, 0.0f, 0.0f, 0.0f, //
                          0.0f, 1.0f, 0.0f, 0.0f, //
                          0.0f, 0.0f, 1.0f, 0.0f, //
                          x, y, z, 1.0f);
        }
        public static Matrix CreateTranslation(Vector3 translation)
        {
            return new Matrix(1.0f, 0.0f, 0.0f, 0.0f, //
                      0.0f, 1.0f, 0.0f, 0.0f, //
                      0.0f, 0.0f, 1.0f, 0.0f, //
                      translation.x, translation.y, translation.z, 1.0f);
        }
        public static Matrix CreateScale(float scaleX, float scaleY,
                                     float scaleZ)
        {
            return new Matrix(scaleX, 0, 0, 0, //
                              0, scaleY, 0, 0, //
                              0, 0, scaleZ, 0, //
                              0, 0, 0, 1);
        }
        public static Matrix CreateScale(Vector3 scale)
        {
            return CreateScale(scale.x, scale.y, scale.z);
        }

        public static Matrix CreateRotationX(float _angle)
        {
            float sinAngle, cosAngle;
            sinAngle = (float)Math.Sin(_angle);
            cosAngle = (float)Math.Cos(_angle);

            return new Matrix(1.0f, 0.0f, 0.0f, 0.0f,          //
                              0.0f, cosAngle, sinAngle, 0.0f,  //
                              0.0f, -sinAngle, cosAngle, 0.0f, //
                              0.0f, 0.0f, 0.0f, 1.0f);
        }
        public static Matrix CreateRotationY(float _angle)
        {
            float sinAngle, cosAngle;
            sinAngle = (float)Math.Sin(_angle);
            cosAngle = (float)Math.Cos(_angle);

            return new Matrix(cosAngle, 0.0f, -sinAngle, 0.0f, //
                              0.0f, 1.0f, 0.0f, 0.0f,          //
                              sinAngle, 0.0f, cosAngle, 0.0f,  //
                              0.0f, 0.0f, 0.0f, 1.0f);
        }
        public static Matrix CreateRotationZ(float _angle)
        {
            float sinAngle, cosAngle;
            sinAngle = (float)Math.Sin(_angle);
            cosAngle = (float)Math.Cos(_angle);

            return new Matrix(cosAngle, sinAngle, 0.0f, 0.0f,  //
                              -sinAngle, cosAngle, 0.0f, 0.0f, //
                              0.0f, 0.0f, 1.0f, 0.0f,          //
                              0.0f, 0.0f, 0.0f, 1.0f);
        }

        public static Matrix CreateFromQuaternion(Quaternion q)
        {
            float xx = q.x * q.x;
            float yy = q.y * q.y;
            float zz = q.z * q.z;
            float xy = q.x * q.y;
            float zw = q.z * q.w;
            float zx = q.z * q.x;
            float yw = q.y * q.w;
            float yz = q.y * q.z;
            float xw = q.x * q.w;

            return new Matrix(
                1.0f - (2.0f * (yy + zz)), 2.0f * (xy + zw), 2.0f * (zx - yw), 0.0f, //
                2.0f * (xy - zw), 1.0f - (2.0f * (zz + xx)), 2.0f * (yz + xw), 0.0f, //
                2.0f * (zx + yw), 2.0f * (yz - xw), 1.0f - (2.0f * (yy + xx)), 0.0f, //
                0.0f, 0.0f, 0.0f, 1.0f);
        }

        public static Matrix CreateFromYawPitchRoll(float _yaw, float _pitch,
                                                float _roll)
        {
            // roatation order is ZXY
            Matrix result = new Matrix();

            result *= CreateRotationZ(_roll);
            result *= CreateRotationX(_pitch);
            result *= CreateRotationY(_yaw);

            return result;
        }
        public static Matrix CreateFromYawPitchRoll(Vector3 angles)
        {
            Matrix result = new Matrix();

            result *= CreateRotationZ(angles.z);
            result *= CreateRotationX(angles.x);
            result *= CreateRotationY(angles.y);

            return result;
        }

        // 透視射影行列を作成する。（左手座標系）
        public static Matrix CreatePerspectiveFovLH(float fov, float aspectRatio,
                                                    float near, float far)
        {
            // std::cosf(0.5f * _fov) / std::sinf(0.5f * _fov)
            float height = 1.0f / (float)Math.Tan(0.5f * fov);
            float width = height / aspectRatio;
            float range = far / (far - near);

            return new Matrix(width, 0, 0, 0,  //
                              0, height, 0, 0, //
                              0, 0, range, 0,  //
                              0, 0, -range * near, 1);
        }

        // 正射影行列を作成する。（左手座標系）
        public static Matrix CreateOrthoLH(float width, float height, float near,
                                           float far)
        {
            return new Matrix(2.0f / width, 0.0f, 0.0f, 0.0f,         //
                  0.0f, 2.0f / height, 0.0f, 0.0f,        //
                  0.0f, 0.0f, 1.0f / (far - near), 0.0f, //
                  0.0f, 0.0f, -near / (near - far), 1.0f);

        }

        public static Matrix CreateLookAtLH(Vector3 position,
									Vector3 target,
									Vector3 up)
        {

            Vector3 xAxis, yAxis, zAxis;

            zAxis = target - position;
            zAxis.Normalize();

            xAxis = Vector3.Cross(up, zAxis);
            xAxis.Normalize();

            yAxis = Vector3.Cross(zAxis, xAxis);

            return new Matrix(xAxis.x, yAxis.x, zAxis.x, 0, //
                          xAxis.y, yAxis.y, zAxis.y, 0, //
                          xAxis.z, yAxis.z, zAxis.z, 0, //
                          -Vector3.Dot(xAxis, position),
                          -Vector3.Dot(yAxis, position),
                          -Vector3.Dot(zAxis, position), 1.0f);
        }


	
	    public static Matrix Lerp(Matrix start, Matrix end, float t)
        {
            Matrix result = new Matrix();
            for (int i = 0; i < 16; ++i)
            {
                result[i] = Mathf.Lerp(start[i], end[i], t);
            }

            return result;
        }

        public override bool Equals(object obj)
        {
            return obj is Matrix matrix &&
                   m11 == matrix.m11 &&
                   m12 == matrix.m12 &&
                   m13 == matrix.m13 &&
                   m14 == matrix.m14 &&
                   m21 == matrix.m21 &&
                   m22 == matrix.m22 &&
                   m23 == matrix.m23 &&
                   m24 == matrix.m24 &&
                   m31 == matrix.m31 &&
                   m32 == matrix.m32 &&
                   m33 == matrix.m33 &&
                   m34 == matrix.m34 &&
                   m41 == matrix.m41 &&
                   m42 == matrix.m42 &&
                   m43 == matrix.m43 &&
                   m44 == matrix.m44;
        }
        public bool Equals(Matrix other)
        {
            return m11 == other.m11 &&
                   m12 == other.m12 &&
                   m13 == other.m13 &&
                   m14 == other.m14 &&
                   m21 == other.m21 &&
                   m22 == other.m22 &&
                   m23 == other.m23 &&
                   m24 == other.m24 &&
                   m31 == other.m31 &&
                   m32 == other.m32 &&
                   m33 == other.m33 &&
                   m34 == other.m34 &&
                   m41 == other.m41 &&
                   m42 == other.m42 &&
                   m43 == other.m43 &&
                   m44 == other.m44;
        }

        public override int GetHashCode()
        {
            int hashCode = -414130488;
            hashCode = hashCode * -1521134295 + m11.GetHashCode();
            hashCode = hashCode * -1521134295 + m12.GetHashCode();
            hashCode = hashCode * -1521134295 + m13.GetHashCode();
            hashCode = hashCode * -1521134295 + m14.GetHashCode();
            hashCode = hashCode * -1521134295 + m21.GetHashCode();
            hashCode = hashCode * -1521134295 + m22.GetHashCode();
            hashCode = hashCode * -1521134295 + m23.GetHashCode();
            hashCode = hashCode * -1521134295 + m24.GetHashCode();
            hashCode = hashCode * -1521134295 + m31.GetHashCode();
            hashCode = hashCode * -1521134295 + m32.GetHashCode();
            hashCode = hashCode * -1521134295 + m33.GetHashCode();
            hashCode = hashCode * -1521134295 + m34.GetHashCode();
            hashCode = hashCode * -1521134295 + m41.GetHashCode();
            hashCode = hashCode * -1521134295 + m42.GetHashCode();
            hashCode = hashCode * -1521134295 + m43.GetHashCode();
            hashCode = hashCode * -1521134295 + m44.GetHashCode();
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

            return string.Format("({0}, {1}, {2}, {3}, \n{4}, {5}, {6}, {7}, \n{8}, {9}, {10}, {11}, \n{12}, {13}, {14}, {15}\n)",
                m11.ToString(format), m12.ToString(format), m13.ToString(format), m14.ToString(format), 
                m21.ToString(format), m22.ToString(format), m23.ToString(format), m24.ToString(format),
                m31.ToString(format), m32.ToString(format), m33.ToString(format), m34.ToString(format),
                m41.ToString(format), m42.ToString(format), m43.ToString(format), m44.ToString(format));
        }


        // 演算子
        public static bool operator ==(Matrix lhs, Matrix rhs)
        {
            for (int i = 0; i < 16; ++i)
            {
                if (lhs[i] != rhs[i])
                {
                    return false;
                }
            }
            return true;
        }
        public static bool operator !=(Matrix lhs, Matrix rhs)
        {
            for (int i = 0; i < 16; ++i)
            {
                if (lhs[i] != rhs[i])
                {
                    return true;
                }
            }
            return false;
        }

	    public static Matrix operator *(Matrix lhs, Matrix rhs)
        {
            Matrix result = new Matrix();

            result.m11 =lhs.m11 * rhs.m11 +lhs.m12 * rhs.m21 +lhs.m13 * rhs.m31 +lhs.m14 * rhs.m41;
            result.m12 =lhs.m11 * rhs.m12 +lhs.m12 * rhs.m22 +lhs.m13 * rhs.m32 +lhs.m14 * rhs.m42;
            result.m13 =lhs.m11 * rhs.m13 +lhs.m12 * rhs.m23 +lhs.m13 * rhs.m33 +lhs.m14 * rhs.m43;
            result.m14 =lhs.m11 * rhs.m14 +lhs.m12 * rhs.m24 +lhs.m13 * rhs.m34 +lhs.m14 * rhs.m44;

            result.m21 =lhs.m21 * rhs.m11 +lhs.m22 * rhs.m21 +lhs.m23 * rhs.m31 +lhs.m24 * rhs.m41;
            result.m22 =lhs.m21 * rhs.m12 +lhs.m22 * rhs.m22 +lhs.m23 * rhs.m32 +lhs.m24 * rhs.m42;
            result.m23 =lhs.m21 * rhs.m13 +lhs.m22 * rhs.m23 +lhs.m23 * rhs.m33 +lhs.m24 * rhs.m43;
            result.m24 =lhs.m21 * rhs.m14 +lhs.m22 * rhs.m24 +lhs.m23 * rhs.m34 +lhs.m24 * rhs.m44;

            result.m31 =lhs.m31 * rhs.m11 +lhs.m32 * rhs.m21 +lhs.m33 * rhs.m31 +lhs.m34 * rhs.m41;
            result.m32 =lhs.m31 * rhs.m12 +lhs.m32 * rhs.m22 +lhs.m33 * rhs.m32 +lhs.m34 * rhs.m42;
            result.m33 =lhs.m31 * rhs.m13 +lhs.m32 * rhs.m23 +lhs.m33 * rhs.m33 +lhs.m34 * rhs.m43;
            result.m34 =lhs.m31 * rhs.m14 +lhs.m32 * rhs.m24 +lhs.m33 * rhs.m34 +lhs.m34 * rhs.m44;

            result.m41 =lhs.m41 * rhs.m11 +lhs.m42 * rhs.m21 +lhs.m43 * rhs.m31 +lhs.m44 * rhs.m41;
            result.m42 =lhs.m41 * rhs.m12 +lhs.m42 * rhs.m22 +lhs.m43 * rhs.m32 +lhs.m44 * rhs.m42;
            result.m43 =lhs.m41 * rhs.m13 +lhs.m42 * rhs.m23 +lhs.m43 * rhs.m33 +lhs.m44 * rhs.m43;
            result.m44 =lhs.m41 * rhs.m14 +lhs.m42 * rhs.m24 +lhs.m43 * rhs.m34 +lhs.m44 * rhs.m44;

            return result;
        }
    }
}
