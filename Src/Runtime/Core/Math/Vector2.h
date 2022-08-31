#pragma once

namespace Miyadaiku
{
/*
 * ２次元ベクトルクラス。
 *
 */
struct Vector2
{
	float x;
	float y;

	Vector2();
	Vector2(float _x, float _y);

	// Constants
	static const Vector2 Zero;
	static const Vector2 One;
	static const Vector2 UnitX;
	static const Vector2 UnitY;

	float Length() const;
	float LengthSquared() const;

	float Dot(const Vector2& _v) const;
	// Vector2 Cross(const Vector2& _v) const;

	void Normalize();

	// 線形補間
	static Vector2 Lerp(Vector2& _start, Vector2& _end, float _t);

	// 演算子
	bool operator==(const Vector2& _v) const;
	bool operator!=(const Vector2& _v) const;

	Vector2& operator+=(const Vector2& _v);
	Vector2	 operator+(const Vector2& _v) const;

	Vector2& operator-=(const Vector2& _v);
	Vector2	 operator-(const Vector2& _v) const;

	Vector2& operator*=(const Vector2& _v);
	Vector2	 operator*(const Vector2& _v) const;
	Vector2& operator*=(float _val);
	Vector2	 operator*(float _val) const;

	Vector2& operator/=(const Vector2& _v);
	Vector2	 operator/(const Vector2& _v) const;
	Vector2& operator/=(float _val);
	Vector2	 operator/(float _val) const;
};

inline bool Vector2::operator==(const Vector2& _v) const
{
	return (x == _v.x && y == _v.y);
}

inline bool Vector2::operator!=(const Vector2& _v) const
{
	return (x != _v.x || y != _v.y);
}

inline Vector2& Vector2::operator+=(const Vector2& _v)
{
	x += _v.x;
	y += _v.y;
	return (*this);
}

inline Vector2 Vector2::operator+(const Vector2& _v) const
{
	return Vector2(x + _v.x, y + _v.y);
}

inline Vector2& Vector2::operator-=(const Vector2& _v)
{
	x -= _v.x;
	y -= _v.y;
	return (*this);
}

inline Vector2 Vector2::operator-(const Vector2& _v) const
{
	return Vector2(x - _v.x, y - _v.y);
}

inline Vector2& Vector2::operator*=(const Vector2& _v)
{
	x *= _v.x;
	y *= _v.y;
	return (*this);
}

inline Vector2 Vector2::operator*(const Vector2& _v) const
{
	return Vector2(x * _v.x, y * _v.y);
}

inline Vector2& Vector2::operator*=(float _val)
{
	x *= _val;
	y *= _val;
	return (*this);
}

inline Vector2 Vector2::operator*(float _val) const
{
	return Vector2(x + _val, y + _val);
}

inline Vector2& Vector2::operator/=(const Vector2& _v)
{
	x /= _v.x;
	y /= _v.y;
	return (*this);
}

inline Vector2 Vector2::operator/(const Vector2& _v) const
{
	return Vector2(x / _v.x, y / _v.y);
}

inline Vector2& Vector2::operator/=(float _val)
{
	x /= _val;
	y /= _val;
	return (*this);
}

inline Vector2 Vector2::operator/(float _val) const
{
	return Vector2(x / _val, y / _val);
}

} // namespace Miyadaiku