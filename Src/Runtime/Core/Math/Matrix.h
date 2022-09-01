#pragma once
#include "Quaternion.h"
#include "Vector3.h"
#include <cmath>
#include <cassert>

namespace Miyadaiku
{
struct Vector4;
struct Quaternion;

/*
 * 4x4の行列。行優先（Row-major）。
 */
struct Matrix
{
// 無名構造体の警告回避用
#pragma warning(push)
#pragma warning(disable : 4201)
	union
	{
		struct
		{
			float m11, m12, m13, m14;
			float m21, m22, m23, m24;
			float m31, m32, m33, m34;
			float m41, m42, m43, m44;
		};
		float m[4][4];
	};
#pragma warning(pop)

	Matrix();
	Matrix(float _m11, float _m12, float _m13, float _m14, float _m21,
		   float _m22, float _m23, float _m24, float _m31, float _m32,
		   float _m33, float _m34, float _m41, float _m42, float _m43,
		   float _m44);
	Matrix(const float* _array);
	// Matrix(const Vector4& _r1, const Vector4& _r2, const Vector4& _r3,
	//	   const Vector4& _r4);

	static const Matrix Identity;

	Vector3 Up() const;
	Vector3 Down() const;
	Vector3 Right() const;
	Vector3 Left() const;
	Vector3 Foward() const;
	Vector3 Backward() const;

	Matrix Invert() const;

	
	Vector3 GetScale();

	// TODO: 符号付のスケールを返す、GetScaleSignedの実装

	// Quaternion GetRotation();

	Vector3 GetTranslation() const;

	/*
	 * 転置行列を返します。
	 */
	Matrix Transpose() const;
	float  Determinant() const;

	// void Translate(const Vector3& _t);

	//--------------------------------------------

	static inline Matrix CreateTranslation(float _x, float _y, float _z);
	static inline Matrix CreateTranslation(const Vector3& _translation);
	static inline Matrix CreateScale(float _scaleX, float _scaleY,
									 float _scaleZ);
	static inline Matrix CreateScale(const Vector3& _scale);

	static inline Matrix CreateRotationX(float _angle);
	static inline Matrix CreateRotationY(float _angle);
	static inline Matrix CreateRotationZ(float _angle);

	static inline Matrix CreateFromQuaternion(const Quaternion& _q);

	static inline Matrix CreateFromYawPitchRoll(float _yaw, float _pitch,
												float _roll);
	static inline Matrix CreateFromYawPitchRoll(const Vector3& _angles);

	// 透視射影行列を作成する。（左手座標系）
	static inline Matrix CreatePerspectiveFovLH(float _fov, float _aspectRatio,
												float _near, float _far);

	// 正射影行列を作成する。（左手座標系）
	static inline Matrix CreateOrthoLH(float _width, float _height, float _near,
									   float _far);

	static inline Matrix CreateLookAtLH(const Vector3& _position,
										const Vector3& _target,
										const Vector3& _up);

	//
	static Matrix Lerp(const Matrix& _start, const Matrix& _end, float _t);

	// 演算子
	bool operator==(const Matrix& _m) const;
	bool operator!=(const Matrix& _m) const;

	Matrix& operator+=(const Matrix& _m);
	Matrix	operator+(const Matrix& _m) const;

	Matrix& operator-=(const Matrix& _m);
	Matrix	operator-(const Matrix& _m) const;

	Matrix& operator*=(const Matrix& _m);
	Matrix	operator*(const Matrix& _m) const;
	// Matrix& operator*=(float _val);
	// Matrix	 operator*(float _val) const;

	Matrix& operator/=(const Matrix& _m);
	Matrix	operator/(const Matrix& _m) const;
	// Matrix& operator/=(float _val);
	// Matrix	 operator/(float _val) const;
};

inline Matrix Matrix::CreateTranslation(float _x, float _y, float _z)
{
	return Matrix(1.0f, 0.0f, 0.0f, 0.0f, //
				  0.0f, 1.0f, 0.0f, 0.0f, //
				  0.0f, 0.0f, 1.0f, 0.0f, //
				  _x, _y, _z, 1.0f);
}

inline Matrix Matrix::CreateTranslation(const Vector3& _translation)
{
	return Matrix(1.0f, 0.0f, 0.0f, 0.0f, //
				  0.0f, 1.0f, 0.0f, 0.0f, //
				  0.0f, 0.0f, 1.0f, 0.0f, //
				  _translation.x, _translation.y, _translation.z, 1.0f);
}

inline Matrix Matrix::CreateScale(float _scaleX, float _scaleY, float _scaleZ)
{
	return Matrix(_scaleX, 0, 0, 0, //
				  0, _scaleY, 0, 0, //
				  0, 0, _scaleZ, 0, //
				  0, 0, 0, 1);
}

inline Matrix Matrix::CreateScale(const Vector3& _scale)
{
	return CreateScale(_scale.x, _scale.y, _scale.z);
}

inline Matrix Matrix::CreateRotationX(float _angle)
{
	float sinAngle, cosAngle;
	sinAngle = std::sinf(_angle);
	cosAngle = std::cosf(_angle);

	return Matrix(1.0f, 0.0f, 0.0f, 0.0f,		   //
				  0.0f, cosAngle, sinAngle, 0.0f,  //
				  0.0f, -sinAngle, cosAngle, 0.0f, //
				  0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix Matrix::CreateRotationY(float _angle)
{
	float sinAngle, cosAngle;
	sinAngle = std::sinf(_angle);
	cosAngle = std::cosf(_angle);

	return Matrix(cosAngle, 0.0f, -sinAngle, 0.0f, //
				  0.0f, 1.0f, 0.0f, 0.0f,		   //
				  sinAngle, 0.0f, cosAngle, 0.0f,  //
				  0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix Matrix::CreateRotationZ(float _angle)
{
	float sinAngle, cosAngle;
	sinAngle = std::sinf(_angle);
	cosAngle = std::cosf(_angle);

	return Matrix(cosAngle, sinAngle, 0.0f, 0.0f,  //
				  -sinAngle, cosAngle, 0.0f, 0.0f, //
				  0.0f, 0.0f, 1.0f, 0.0f,		   //
				  0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix Matrix::CreateFromQuaternion(const Quaternion& _q)
{
	const float xx = _q.x * _q.x;
	const float yy = _q.y * _q.y;
	const float zz = _q.z * _q.z;
	const float xy = _q.x * _q.y;
	const float zw = _q.z * _q.w;
	const float zx = _q.z * _q.x;
	const float yw = _q.y * _q.w;
	const float yz = _q.y * _q.z;
	const float xw = _q.x * _q.w;

	return Matrix(
		1.0f - (2.0f * (yy + zz)), 2.0f * (xy + zw), 2.0f * (zx - yw), 0.0f, //
		2.0f * (xy - zw), 1.0f - (2.0f * (zz + xx)), 2.0f * (yz + xw), 0.0f, //
		2.0f * (zx + yw), 2.0f * (yz - xw), 1.0f - (2.0f * (yy + xx)), 0.0f, //
		0.0f, 0.0f, 0.0f, 1.0f);
}

inline Matrix Matrix::CreateFromYawPitchRoll(float _yaw, float _pitch,
											 float _roll)
{
	// 回転順は、ZXY(Unityと同じ)。DirectXTK(YXZ)と互換性がないので注意。
	Matrix result;

	result *= CreateRotationZ(_roll);
	result *= CreateRotationX(_pitch);
	result *= CreateRotationY(_yaw);

	return result;
}

inline Matrix Matrix::CreateFromYawPitchRoll(const Vector3& _angles)
{
	Matrix result;

	result *= CreateRotationZ(_angles.z);
	result *= CreateRotationX(_angles.x);
	result *= CreateRotationY(_angles.y);

	return result;
}

inline Matrix Matrix::CreatePerspectiveFovLH(float _fov, float _aspectRatio,
											 float _near, float _far)
{
	// std::cosf(0.5f * _fov) / std::sinf(0.5f * _fov)
	float height = 1.0f / std::tanf(0.5f * _fov);
	float width = height / _aspectRatio;
	float range = _far / (_far - _near);

	return Matrix(width, 0, 0, 0,  //
				  0, height, 0, 0, //
				  0, 0, range, 0,  //
				  0, 0, -range * _near, 1);
}

inline Matrix Matrix::CreateOrthoLH(float _width, float _height, float _near,
									float _far)
{
	return Matrix(2.0f / _width, 0.0f, 0.0f, 0.0f,		   //
				  0.0f, 2.0f / _height, 0.0f, 0.0f,		   //
				  0.0f, 0.0f, 1.0f / (_far - _near), 0.0f, //
				  0.0f, 0.0f, -_near / (_near - _far), 1.0f);
}

inline Matrix Matrix::CreateLookAtLH(const Vector3& _position,
									 const Vector3& _target, const Vector3& _up)
{
	Vector3 xAxis, yAxis, zAxis;

	zAxis = Vector3(_target - _position);
	zAxis.Normalize();

	xAxis = Vector3::Cross(_up, zAxis);
	xAxis.Normalize();

	yAxis = Vector3::Cross(zAxis, xAxis);

	return Matrix(xAxis.x, yAxis.x, zAxis.x, 0, //
				  xAxis.y, yAxis.y, zAxis.y, 0, //
				  xAxis.z, yAxis.z, zAxis.z, 0, //
				  -Vector3::Dot(xAxis, _position),
				  -Vector3::Dot(yAxis, _position),
				  -Vector3::Dot(zAxis, _position), 1.0f);
}

//----------------------------------

inline bool Matrix::operator==(const Matrix& _m) const
{
	const float* left = &m11;
	const float* right = &_m.m11;
	for (int i = 0; i < 16; ++i)
	{
		if (left[i] != right[i])
		{
			return false;
		}
	}
	return true;
}

inline bool Matrix::operator!=(const Matrix& _m) const
{
	const float* left = &m11;
	const float* right = &_m.m11;
	for (int i = 0; i < 16; ++i)
	{
		if (left[i] != right[i])
		{
			return true;
		}
	}
	return false;
}

inline Matrix& Matrix::operator+=(const Matrix& _m)
{
	float*		 left = &m11;
	const float* right = &_m.m11;
	for (int i = 0; i < 16; ++i)
	{
		left[i] += right[i];
	}
	return (*this);
}

inline Matrix Matrix::operator+(const Matrix& _m) const
{
	Matrix		 result;
	float*		 resultData = &result.m11;
	const float* left = &m11;
	const float* right = &_m.m11;
	for (int i = 0; i < 16; ++i)
	{
		resultData[i] = left[i] + right[i];
	}

	return result;
}

inline Matrix& Matrix::operator-=(const Matrix& _m)
{
	float*		 left = &m11;
	const float* right = &_m.m11;
	for (int i = 0; i < 16; ++i)
	{
		left[i] -= right[i];
	}
	return (*this);
}

inline Matrix Matrix::operator-(const Matrix& _m) const
{
	Matrix		 result;
	float*		 resultData = &result.m11;
	const float* left = &m11;
	const float* right = &_m.m11;
	for (int i = 0; i < 16; ++i)
	{
		resultData[i] = left[i] - right[i];
	}

	return result;
}

inline Matrix& Matrix::operator*=(const Matrix& _m)
{
	return (*this) = (*this) * _m;
}

inline Matrix Matrix::operator*(const Matrix& _m) const
{
	Matrix result;

	result.m11 = m11 * _m.m11 + m12 * _m.m21 + m13 * _m.m31 + m14 * _m.m41;
	result.m12 = m11 * _m.m12 + m12 * _m.m22 + m13 * _m.m32 + m14 * _m.m42;
	result.m13 = m11 * _m.m13 + m12 * _m.m23 + m13 * _m.m33 + m14 * _m.m43;
	result.m14 = m11 * _m.m14 + m12 * _m.m24 + m13 * _m.m34 + m14 * _m.m44;

	result.m21 = m21 * _m.m11 + m22 * _m.m21 + m23 * _m.m31 + m24 * _m.m41;
	result.m22 = m21 * _m.m12 + m22 * _m.m22 + m23 * _m.m32 + m24 * _m.m42;
	result.m23 = m21 * _m.m13 + m22 * _m.m23 + m23 * _m.m33 + m24 * _m.m43;
	result.m24 = m21 * _m.m14 + m22 * _m.m24 + m23 * _m.m34 + m24 * _m.m44;

	result.m31 = m31 * _m.m11 + m32 * _m.m21 + m33 * _m.m31 + m34 * _m.m41;
	result.m32 = m31 * _m.m12 + m32 * _m.m22 + m33 * _m.m32 + m34 * _m.m42;
	result.m33 = m31 * _m.m13 + m32 * _m.m23 + m33 * _m.m33 + m34 * _m.m43;
	result.m34 = m31 * _m.m14 + m32 * _m.m24 + m33 * _m.m34 + m34 * _m.m44;

	result.m41 = m41 * _m.m11 + m42 * _m.m21 + m43 * _m.m31 + m44 * _m.m41;
	result.m42 = m41 * _m.m12 + m42 * _m.m22 + m43 * _m.m32 + m44 * _m.m42;
	result.m43 = m41 * _m.m13 + m42 * _m.m23 + m43 * _m.m33 + m44 * _m.m43;
	result.m44 = m41 * _m.m14 + m42 * _m.m24 + m43 * _m.m34 + m44 * _m.m44;

	return result;
}

inline Matrix& Matrix::operator/=(const Matrix& _m)
{
	float*		 left = &m11;
	const float* right = &_m.m11;
	for (int i = 0; i < 16; ++i)
	{
		left[i] /= right[i];
	}
	return (*this);
}

inline Matrix Matrix::operator/(const Matrix& _m) const
{
	Matrix		 result;
	float*		 resultData = &result.m11;
	const float* left = &m11;
	const float* right = &_m.m11;
	for (int i = 0; i < 16; ++i)
	{
		resultData[i] = left[i] / right[i];
	}

	return result;
}
} // namespace Miyadaiku
