#include "Matrix.h"
#include "Vector3.h"
#include "Vector4.h"


namespace Miyadaiku
{
const Matrix Matrix::Identity = Matrix(1.0f, 0.0f, 0.0f, 0.0f, //
									   0.0f, 1.0f, 0.0f, 0.0f, //
									   0.0f, 0.0f, 1.0f, 0.0f, //
									   0.0f, 0.0f, 0.0f, 1.0f);

Matrix::Matrix()
{
	m11 = m22 = m33 = m44 = 1.0f;
	m12 = m13 = m14 = m21 = m23 = m24 = m31 = m32 = m34 = m41 = m42 = m43 =
		0.0f;
}
Matrix::Matrix(float _m11, float _m12, float _m13, float _m14, float _m21,
			   float _m22, float _m23, float _m24, float _m31, float _m32,
			   float _m33, float _m34, float _m41, float _m42, float _m43,
			   float _m44)
{
	m11 = _m11;
	m12 = _m12;
	m13 = _m13;
	m14 = _m14;

	m21 = _m21;
	m22 = _m22;
	m23 = _m23;
	m24 = _m24;

	m31 = _m31;
	m32 = _m32;
	m33 = _m33;
	m34 = _m34;

	m41 = _m41;
	m42 = _m42;
	m43 = _m43;
	m44 = _m44;
}
Matrix::Matrix(const float* _array)
{
	assert(_array != nullptr);

	m[0][0] = _array[0];
	m[0][1] = _array[1];
	m[0][2] = _array[2];
	m[0][3] = _array[3];

	m[1][0] = _array[4];
	m[1][1] = _array[5];
	m[1][2] = _array[6];
	m[1][3] = _array[7];

	m[2][0] = _array[8];
	m[2][1] = _array[9];
	m[2][2] = _array[10];
	m[2][3] = _array[11];

	m[3][0] = _array[12];
	m[3][1] = _array[13];
	m[3][2] = _array[14];
	m[3][3] = _array[15];
}
// Matrix::Matrix(const Vector4& _r1, const Vector4& _r2, const Vector4& _r3,
//			   const Vector4& _r4)
//{
//
// }

inline Vector3 Matrix::Up() const
{
	return Vector3(m21, m22, m23);
}

Vector3 Matrix::Down() const
{
	return Vector3(-m21, -m22, -m23);
}

Vector3 Matrix::Right() const
{
	return Vector3(m11, m12, m13);
}

Vector3 Matrix::Left() const
{
	return Vector3(-m11, -m12, -m13);
}

Vector3 Matrix::Foward() const
{
	return Vector3(m31, m32, m33);
}

Vector3 Matrix::Backward() const
{
	return Vector3(-m31, -m32, -m33);
}

Matrix Matrix::Invert() const
{
	// 吐き出し法による行列演算
	// 参考１：https://sun.ac.jp/prof/hnagano/houkoku/r4linearalgebra-05.html
	// 参考２：https://betashort-lab.com/%E3%83%87%E3%83%BC%E3%82%BF%E3%82%B5%E3%82%A4%E3%82%A8%E3%83%B3%E3%82%B9/%E7%B5%B1%E8%A8%88%E5%AD%A6/%E9%80%86%E8%A1%8C%E5%88%97%E3%81%AE%E6%B1%82%E3%82%81%E6%96%B9-with-python/

	Matrix result(Matrix::Identity);

	Matrix copy(*this);
	float  buf;
	int	   i, j, k;
	//掃き出し法
	for (i = 0; i < 4; i++)
	{
		buf = 1 / copy.m[i][i];
		for (j = 0; j < 4; j++)
		{
			copy.m[i][j] *= buf;
			result.m[i][j] *= buf;
		}
		for (j = 0; j < 4; j++)
		{
			if (i != j)
			{
				buf = copy.m[j][i];
				for (k = 0; k < 4; k++)
				{
					copy.m[j][k] -= copy.m[i][k] * buf;
					result.m[j][k] -= result.m[i][k] * buf;
				}
			}
		}
	}
	return result;
}

float Matrix::Determinant() const
{
	return m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
		 - m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
		 + m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]);
}

Vector3 Matrix::GetScale()
{
	return Vector3(std::sqrtf(m11 * m11 + m12 * m12 + m13 * m13),
				   std::sqrtf(m21 * m21 + m22 * m22 + m23 * m23),
				   std::sqrtf(m31 * m31 + m32 * m32 + m33 * m33));
}

Vector3 Matrix::GetTranslation() const
{
	return Vector3(m41, m42, m43);
}

Matrix Matrix::Transpose() const
{
	Matrix result;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			result.m[i][j] = m[j][i];
		}
	}
	return result;
}

Matrix Lerp(const Matrix& _start, const Matrix& _end, float _t)
{
	Matrix		 result;
	float*		 resultData = &result.m11;
	const float* startData = &_start.m11;
	const float* endData = &_end.m11;
	for (int i = 0; i < 16; ++i)
	{
		resultData[i] = std::lerp(startData[i], endData[i], _t);
	}

	return result;
}

} // namespace Miyadaiku