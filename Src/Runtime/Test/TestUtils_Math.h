#pragma once
#include <gtest/gtest.h>

// テスト時の浮動小数の誤差しきい値
#define FLOAT_THRESHOLD 0.0001f

#define ASSERT_NEAR_FLOAT(_a, _b) ASSERT_NEAR(_a, _b, FLOAT_THRESHOLD);


#define ASSERT_NEAR_VEC2(_x, _y, _v)                                           \
	ASSERT_NEAR(_x, _v.x, FLOAT_THRESHOLD);                                    \
	ASSERT_NEAR(_y, _v.y, FLOAT_THRESHOLD);

#define ASSERT_EQUAL_Vector2I(_x, _y, _val)                                    \
	ASSERT_EQ(_x, _val.x);                                                     \
	ASSERT_EQ(_y, _val.y);

#define ASSERT_NEAR_VEC3(_x, _y, _z, _v)                                       \
	ASSERT_NEAR(_x, _v.x, FLOAT_THRESHOLD);                                    \
	ASSERT_NEAR(_y, _v.y, FLOAT_THRESHOLD);                                    \
	ASSERT_NEAR(_z, _v.z, FLOAT_THRESHOLD);

#define ASSERT_NEAR_VEC4(_x, _y, _z, _w, _v)                                   \
	ASSERT_NEAR(_x, _v.x, FLOAT_THRESHOLD);                                    \
	ASSERT_NEAR(_y, _v.y, FLOAT_THRESHOLD);                                    \
	ASSERT_NEAR(_z, _v.z, FLOAT_THRESHOLD);                                    \
	ASSERT_NEAR(_w, _v.w, FLOAT_THRESHOLD);

#define ASSERT_NEAR_MAT(_M11, _M12, _M13, _M14, _M21, _M22, _M23, _M24, _M31,  \
						_M32, _M33, _M34, _M41, _M42, _M43, _M44, _M)          \
	ASSERT_NEAR(_M11, _M.m11, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M12, _M.m12, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M13, _M.m13, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M14, _M.m14, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M21, _M.m21, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M22, _M.m22, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M23, _M.m23, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M24, _M.m24, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M31, _M.m31, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M32, _M.m32, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M33, _M.m33, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M34, _M.m34, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M41, _M.m41, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M42, _M.m42, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M43, _M.m43, FLOAT_THRESHOLD);                                \
	ASSERT_NEAR(_M44, _M.m44, FLOAT_THRESHOLD);
