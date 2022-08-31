#include "Vector2.h"
#include <cmath>

namespace Miyadaiku
{

const Vector2 Vector2::Zero = Vector2(0, 0);
const Vector2 Vector2::One = Vector2(1, 1);
const Vector2 Vector2::UnitX = Vector2(1, 0);
const Vector2 Vector2::UnitY = Vector2(0, 1);

Vector2::Vector2()
{
	x = y = 0.0f;
}

Vector2::Vector2(float _x, float _y)
{
	x = _x;
	y = _y;
}

float Vector2::Length() const
{
	return std::sqrtf(x * x + y * y);
}

float Vector2::LengthSquared() const
{
	return (x * x + y * y);
}

float Vector2::Dot(const Vector2& _v) const
{
	return (x * _v.x + y * _v.y);
}

void Vector2::Normalize()
{
	float l = Length();
	if (l == 0.0f)
	{
		return;
	}
	l = 1.0f / l;
	x *= l;
	y *= l;
}

Vector2 Vector2::Lerp(Vector2& _start, Vector2& _end, float _t)
{
	return Vector2(std::lerp(_start.x, _end.x, _t),
				   std::lerp(_start.y, _end.y, _t));
}

// Vector2 Vector2::Cross(const Vector2& _v) const
//{
//	return Vector2(x * _v.y + y * _v.x);
// }

} // namespace Miyadaiku