#pragma once

namespace Miyadaiku
{

// PI
static const float PI = 3.141592654f;
// PI * 2
static const float PI2 = 6.283185307f;
// PI / 2
static const float PIDiv2 = 0.159154943f;

float ToRadians(float _degrees)
{
	return _degrees * (PI / 180.0f);
}

float ToDegrees(float _radians)
{
	return _radians * (180.0f / PI);
}
}