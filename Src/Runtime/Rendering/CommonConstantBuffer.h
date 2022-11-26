#pragma once
#include "../Core/Math/Vector3.h"
#include "../Core/Math/Vector4.h"
#include "../Core/Math/Matrix.h"

namespace Miyadaiku
{
	
struct Cb_Camera
{
	Matrix	mView;		// View matrix
	Matrix	mProj;		// Projection matrix
	Matrix	mViewProj;	// View matrix * Projection matrix
};

struct Cb_Uber
{
	Matrix mWorld; // world matrix
};

struct Cb_Light
{
	Vector4 color;
};

struct Cb_Material
{
	Vector4 baseColor = {1, 1, 1, 1};
};

} // namespace Miyadaiku