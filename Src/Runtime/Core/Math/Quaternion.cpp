#include "Quaternion.h"
#include "Math.h"

namespace Miyadaiku
{
const Quaternion Quaternion::Identity = Quaternion(0.0f, 0.0f, 0.0f, 1.0f);

Quaternion::Quaternion()
{
	x = y = z = 0.0f;
	w = 1.0f;
}

Quaternion::Quaternion(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}

Quaternion Quaternion::Invert() const
{
	float t = 1.0f / ((x * x) + (y * y) + (z * z) + (w * w));
	return Quaternion(-x * t, -y * t, -z * t, w * t);
}

Vector3 Quaternion::ToEulerAngles() const
{
	const float sx = 2.0f * (-y * z + w * x);

	// ジンバルロック回避
	if (sx < -0.9999f)
	{
		return Vector3(
			-PIDiv2, 0.0f,
			-atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z))
				);
	}
	if (sx > 0.9999f)
	{
		return Vector3(
			PIDiv2, 0.0f,
			atan2f(2.0f * (x * z - w * y), 1.0f - 2.0f * (y * y + z * z)));
	}

	return Vector3(
		asinf(sx),
		atan2f(2.0f * (x * z + w * y), 1.0f - 2.0f * (x * x + y * y)),
		atan2f(2.0f * (x * y + w * z), 1.0f - 2.0f * (x * x + z * z)));
}

} // namespace Miyadaiku