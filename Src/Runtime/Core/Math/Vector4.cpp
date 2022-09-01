#include "Vector4.h"


namespace Miyadaiku
{
Vector4::Vector4()
{
	x = y = z = w = 0.0f;
}
Vector4::Vector4(float _x, float _y, float _z, float _w)
{
	x = _x;
	y = _y;
	z = _z;
	w = _w;
}
} // namespace Miyadaiku