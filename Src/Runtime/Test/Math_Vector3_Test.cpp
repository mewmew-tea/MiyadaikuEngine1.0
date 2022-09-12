#include "../Core/Math/Vector3.h"
#include "TestUtils_Math.h"
#include "TestCommon.h"

using namespace Miyadaiku;

namespace Math
{

class Vector3_Test : public ::testing::Test
{
protected:
	virtual void SetUp() override
	{
	}
	virtual void TearDown() override
	{
	}
};

TEST_F(Vector3_Test, Calculation)
{
	// コンストラクタ
	{
		Vector3 v1;
		ASSERT_NEAR_VEC3(0.0f, 0.0f, 0.0f, v1);
		// 引数付き
		Vector3 v2(-1.0f, 1.0f, 0.0f);
		ASSERT_NEAR_VEC3(-1.0f, 1.0f, 0.0f, v2);
	}

	// Length
	{
		Vector3 v1(-1.0f, 2.0f, 3.0f);
		ASSERT_NEAR_FLOAT(3.74165738f, v1.Length());
		ASSERT_NEAR_FLOAT(14.0f, v1.LengthSquared());
	}

	// Dot
	{
		Vector3 v1(1.0f, 2.0f, 3.0f);
		Vector3 v2(2.0f, 4.0f, 5.0f);

		ASSERT_NEAR_FLOAT(25.0f, v1.Dot(v2));
	}

	// Cross
	{
		Vector3 v1(1.0f, 2.0f, 3.0f);
		Vector3 v2(2.0f, 4.0f, 5.0f);

		ASSERT_NEAR_VEC3(-2, 1, 0, v1.Cross(v2));
	}

	// Lerp
	{
		Vector3 v1(0.0f, 0.0f, 20.0f);
		Vector3 v2(30.0f, -60.0f, 80.0f);

		ASSERT_NEAR_VEC3(15.0f, -30.0f, 50.0f, Vector3::Lerp(v1, v2, 0.5f));
		ASSERT_NEAR_VEC3(10.0f, -20.0f, 40.0f,
						 Vector3::Lerp(v1, v2, 1.0f / 3.0f));
	}

	// 演算子
	{
		Vector3 v1(1.0f, 2.0f, 3.0f);
		Vector3 v2(1.0f, 2.0f, 3.0f);
		ASSERT_EQ(true, v1 == v2);
		ASSERT_EQ(false, v1 != v2);
	}
	// 加算
	{
		Vector3 v1(2.0f, 3.0f, 4.0f);
		Vector3 sum = v1 + v1;
		ASSERT_NEAR_VEC3(4.0f, 6.0f, 8.0f, sum);
		v1 += v1;
		ASSERT_NEAR_VEC3(4.0f, 6.0f, 8.0f, v1);
	}
	// 乗算
	{
		Vector3 v1(2.0f, 3.0f, 4.0f);
		Vector3 mul = v1 * v1;
		ASSERT_NEAR_VEC3(4.0f, 9.0f, 16.0f, mul);
		v1 *= v1;
		ASSERT_NEAR_VEC3(4.0f, 9.0f, 16.0f, v1);
	}
	// 除算
	{
		Vector3 v1(2.0f, 3.0f, 4.0f);
		Vector3 mul = v1 / v1;
		ASSERT_NEAR_VEC3(1.0f, 1.0f, 1.0f, mul);
		v1 /= v1;
		ASSERT_NEAR_VEC3(1.0f, 1.0f, 1.0f, v1);
	}
	// 減算
	{
		Vector3 v1(2.0f, 3.0f, 4.0f);
		Vector3 mul = v1 - v1;
		ASSERT_NEAR_VEC3(0.0f, 0.0f, 0.0f, mul);
		v1 -= v1;
		ASSERT_NEAR_VEC3(0.0f, 0.0f, 0.0f, v1);
	}
}

} // namespace Math