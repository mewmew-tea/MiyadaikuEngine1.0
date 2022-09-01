#pragma once
#include "Vector3.h"

#include <cassert>
#include <cmath>

namespace Miyadaiku
{
/*
* 四元数。
* 回転順は、ZXY(Unityと同じ)。DirectXTK(YXZ)と互換性がないので注意。
*/
struct Quaternion
{
	float x;
	float y;
	float z;
	float w;

	Quaternion();
	Quaternion(float _x, float _y, float _z, float _w);

	
    // Constants
	static const Quaternion Identity;


	Quaternion Invert() const;
	Vector3	   ToEulerAngles() const;


	static inline Quaternion CreateFromYawPitchRoll(float _yaw, float _pitch,
													float _roll);

	static inline Quaternion CreateFromEulerAngles(float _x, float _y,
												   float _z);
	static inline Quaternion CreateFromEulerAngles(const Vector3& _angles);

	//static inline Quaternion CreateRotationX(float _angle);
	//static inline Quaternion CreateRotationY(float _angle);
	//static inline Quaternion CreateRotationZ(float _angle);

	// 
	bool operator==(const Quaternion& _v) const;
	bool operator!=(const Quaternion& _v) const;

	Quaternion& operator+=(const Quaternion& _v);
	Quaternion	operator+(const Quaternion& _v) const;

	Quaternion& operator-=(const Quaternion& _v);
	Quaternion	operator-(const Quaternion& _v) const;

	Quaternion& operator*=(const Quaternion& _v);
	Quaternion	operator*(const Quaternion& _v) const;

	Quaternion& operator/=(const Quaternion& _v);
	Quaternion	operator/(const Quaternion& _v) const;

};

inline Quaternion Quaternion::CreateFromYawPitchRoll(float _yaw, float _pitch,
													 float _roll)
{
	return CreateFromEulerAngles(_pitch, _yaw, _roll);
}

inline Quaternion Quaternion::CreateFromEulerAngles(float _x, float _y,
													float _z)
{
	const float halfX = _x * 0.5f;
	const float sinX = std::sinf(halfX);
	const float cosX = std::cosf(halfX);

	const float halfY = _y * 0.5f;
	const float sinY = std::sinf(halfY);
	const float cosY = std::cosf(halfY);

	const float halfZ = _z * 0.5f;
	const float sinZ = std::sinf(halfZ);
	const float cosZ = std::cosf(halfZ);

	return Quaternion(
		(cosY * sinX * cosZ) + (sinY * cosX * sinZ),
		(sinY * cosX * cosZ) - (cosY * sinX * sinZ),
		(cosY * cosX * sinZ) - (sinY * sinX * cosZ),
		(cosY * cosX * cosZ) + (sinY * sinX * sinZ));
}

inline Quaternion Quaternion::CreateFromEulerAngles(const Vector3& _angles)
{
	return CreateFromEulerAngles(_angles.x, _angles.y, _angles.z);
}

inline bool Quaternion::operator==(const Quaternion& _v) const
{
	return (x == _v.x && y == _v.y && z == _v.z && w == _v.w);
}

inline bool Quaternion::operator!=(const Quaternion& _v) const
{
	return (x != _v.x || y != _v.y || z != _v.z || w != _v.w);
}

inline Quaternion& Quaternion::operator+=(const Quaternion& _v)
{
	x += _v.x;
	y += _v.y;
	z += _v.z;
	w += _v.w;
	return (*this);
}

inline Quaternion Quaternion::operator+(const Quaternion& _v) const
{
	return Quaternion(x + _v.x, y + _v.y, z + _v.z, w + _v.w);
}

inline Quaternion& Quaternion::operator-=(const Quaternion& _v)
{
	x -= _v.x;
	y -= _v.y;
	z -= _v.z;
	w -= _v.w;
	return (*this);
}

inline Quaternion Quaternion::operator-(const Quaternion& _v) const
{
	return Quaternion(x - _v.x, y - _v.y, z - _v.z, w - _v.w);
}

inline Quaternion& Quaternion::operator*=(const Quaternion& _v)
{
	const float copyX = x;
	const float copyY = y;
	const float copyZ = z;
	const float copyW = w;

	x = _v.w * copyX + _v.x * copyW + _v.y * copyZ - _v.z * copyY;
	y = _v.w * copyY - _v.x * copyZ + _v.y * copyW + _v.z * copyX;
	z = _v.w * copyZ + _v.x * copyY - _v.y * copyX + _v.z * copyW;
	w = _v.w * copyW - _v.x * copyX - _v.y * copyY - _v.z * copyZ;

	return (*this);
}

inline Quaternion Quaternion::operator*(const Quaternion& _v) const
{
	return Quaternion(_v.w * x + _v.x * w + _v.y * z - _v.z * y,
					  _v.w * y - _v.x * z + _v.y * w + _v.z * x,
					  _v.w * z + _v.x * y - _v.y * x + _v.z * w,
					  _v.w * w - _v.x * x - _v.y * y - _v.z * z);
}

inline Quaternion& Quaternion::operator/=(const Quaternion& _v)
{
	x /= _v.x;
	y /= _v.y;
	z /= _v.z;
	w /= _v.w;
	return (*this);
}

inline Quaternion Quaternion::operator/(const Quaternion& _v) const
{
	return Quaternion(x / _v.x, y / _v.y, z / _v.z, w / _v.w);
}

} // namespace Miyadaiku