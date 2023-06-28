#include "Vector3.h"
#include "Matrix.h"
#include <cmath>

namespace Miyadaiku
{
const Vector3 Vector3::Zero = Vector3(0, 0, 0);
const Vector3 Vector3::One = Vector3(1, 1, 1);
const Vector3 Vector3::UnitX = Vector3(1, 0, 0);
const Vector3 Vector3::UnitY = Vector3(0, 1, 0);
const Vector3 Vector3::UnitZ = Vector3(0, 0, 1);
const Vector3 Vector3::Up = Vector3(0, 1, 0);
const Vector3 Vector3::Down = Vector3(0, -1, 0);
const Vector3 Vector3::Right = Vector3(1, 0, 0);
const Vector3 Vector3::Left = Vector3(-1, 0, 0);
const Vector3 Vector3::Forward = Vector3(0, 0, 1);
const Vector3 Vector3::Backward = Vector3(0, 0, -1);

Vector3::Vector3()
{
	x = y = z = 0.0f;
}

Vector3::Vector3(float _x, float _y, float _z)
{
	x = _x;
	y = _y;
	z = _z;
}

float Vector3::Length() const
{
	return std::sqrtf(x * x + y * y + z * z);
}

float Vector3::LengthSquared() const
{
	return (x * x + y * y + z * z);
}

float Vector3::Dot(const Vector3& _v) const
{
	return (x * _v.x + y * _v.y + z * _v.z);
}

float Vector3::Dot(const Vector3& _v1, const Vector3& _v2)
{
	return (_v1.x * _v2.x + _v1.y * _v2.y + _v1.z * _v2.z);
}

Vector3 Vector3::Cross(const Vector3& _v) const
{
	return Vector3((y * _v.z) - (z * _v.y), (z * _v.x) - (x * _v.z),
				   (x * _v.y) - (y * _v.x));
}

Vector3 Vector3::Cross(const Vector3& _v1, const Vector3& _v2)
{
	return Vector3((_v1.y * _v2.z) - (_v1.z * _v2.y),
				   (_v1.z * _v2.x) - (_v1.x * _v2.z),
				   (_v1.x * _v2.y) - (_v1.y * _v2.x));
}

void Vector3::Normalize()
{
	float l = Length();
	if (l == 0.0f)
	{
		return;
	}
	l = 1.0f / l;
	x *= l;
	y *= l;
	z *= l;
}

Vector3 Vector3::TransformNormal(const Vector3& _v, const Matrix& _m)
{
	Vector3 result;
	result = Vector3(_m.m21 * _v.y, _m.m22 * _v.y, _m.m23 * _v.y);
	result += Vector3(_m.m11 * _v.x, _m.m12 * _v.x, _m.m13 * _v.x);
	result += Vector3(_m.m31 * _v.z, _m.m32 * _v.z, _m.m33 * _v.z);

	return result;
}

Vector3 Vector3::Lerp(Vector3& _start, Vector3& _end, float _t)
{
	return Vector3(std::lerp(_start.x, _end.x, _t),
				   std::lerp(_start.y, _end.y, _t),
				   std::lerp(_start.z, _end.z, _t));
}

} // namespace Miyadaiku