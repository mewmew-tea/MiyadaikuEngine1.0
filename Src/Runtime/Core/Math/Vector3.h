#pragma once

namespace Miyadaiku
{
struct Vector3
{
	float x;
	float y;
	float z;

	Vector3();
	Vector3(float _x, float _y, float _z);

	// Constants
	static const Vector3 Zero;
	static const Vector3 One;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 Up;
	static const Vector3 Down;
	static const Vector3 Right;
	static const Vector3 Left;
	static const Vector3 Forward;
	static const Vector3 Backward;

	float Length() const;
	float LengthSquared() const;

	float		   Dot(const Vector3& _v) const;
	static float   Dot(const Vector3& _v1, const Vector3& _v2);
	Vector3		   Cross(const Vector3& _v) const;
	static Vector3 Cross(const Vector3& _v1, const Vector3& _v2);

	void Normalize();

	// 線形補間
	static Vector3 Lerp(Vector3& _start, Vector3& _end, float _t);

	// 演算子
	bool operator==(const Vector3& _v) const;
	bool operator!=(const Vector3& _v) const;

	Vector3& operator+=(const Vector3& _v);
	Vector3	 operator+(const Vector3& _v) const;

	Vector3& operator-=(const Vector3& _v);
	Vector3	 operator-(const Vector3& _v) const;

	Vector3& operator*=(const Vector3& _v);
	Vector3	 operator*(const Vector3& _v) const;
	Vector3& operator*=(float _val);
	Vector3	 operator*(float _val) const;

	Vector3& operator/=(const Vector3& _v);
	Vector3	 operator/(const Vector3& _v) const;
	Vector3& operator/=(float _val);
	Vector3	 operator/(float _val) const;
};

inline bool Vector3::operator==(const Vector3& _v) const
{
	return (x == _v.x && y == _v.y && z == _v.z);
}

inline bool Vector3::operator!=(const Vector3& _v) const
{
	return (x != _v.x || y != _v.y || z != _v.z);
}

inline Vector3& Vector3::operator+=(const Vector3& _v)
{
	x += _v.x;
	y += _v.y;
	z += _v.z;
	return (*this);
}

inline Vector3 Vector3::operator+(const Vector3& _v) const
{
	return Vector3(x + _v.x, y + _v.y, z + _v.z);
}

inline Vector3& Vector3::operator-=(const Vector3& _v)
{
	x -= _v.x;
	y -= _v.y;
	z -= _v.z;
	return (*this);
}

inline Vector3 Vector3::operator-(const Vector3& _v) const
{
	return Vector3(x - _v.x, y - _v.y, z - _v.z);
}

inline Vector3& Vector3::operator*=(const Vector3& _v)
{
	x *= _v.x;
	y *= _v.y;
	z *= _v.z;
	return (*this);
}

inline Vector3 Vector3::operator*(const Vector3& _v) const
{
	return Vector3(x * _v.x, y * _v.y, z * _v.z);
}

inline Vector3& Vector3::operator*=(float _val)
{
	x *= _val;
	y *= _val;
	z *= _val;
	return (*this);
}

inline Vector3 Vector3::operator*(float _val) const
{
	return Vector3(x * _val, y * _val, z * _val);
}

inline Vector3& Vector3::operator/=(const Vector3& _v)
{
	x /= _v.x;
	y /= _v.y;
	z /= _v.z;
	return (*this);
}

inline Vector3 Vector3::operator/(const Vector3& _v) const
{
	return Vector3(x / _v.x, y / _v.y, z / _v.z);
}

inline Vector3& Vector3::operator/=(float _val)
{
	x /= _val;
	y /= _val;
	z /= _val;
	return (*this);
}

inline Vector3 Vector3::operator/(float _val) const
{
	return Vector3(x / _val, y / _val, z / _val);
}

} // namespace Miyadaiku