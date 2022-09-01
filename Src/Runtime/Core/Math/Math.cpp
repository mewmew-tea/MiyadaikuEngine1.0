#include "Math.h"

namespace Miyadaiku
{
float ToRadians(float _degrees)
{
	return _degrees * (PI / 180.0f);
}
float ToDegrees(float _radians)
{
	return _radians * (180.0f / PI);
}
} // namespace Miyadaiku