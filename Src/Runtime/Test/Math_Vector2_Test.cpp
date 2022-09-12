#include "../Core/Math/Vector2.h"
#include "TestUtils_Math.h"
#include "TestCommon.h"

using namespace Miyadaiku;

namespace Math
{

class Vector2_Test : public ::testing::Test
{
protected:
	virtual void SetUp() override
	{
	}
	virtual void TearDown() override
	{
	}
};

TEST_F(Vector2_Test, Calculation)
{
	// コンストラクタ
	{
		Vector2 v1;
		ASSERT_NEAR_VEC2(0.0f, 0.0f, v1);
		// 引数付き
		Vector2 v2(-1.0f, 1.0f);
		ASSERT_NEAR_VEC2(-1.0f, 1.0f, v2);
	}

	// Length
	{
		Vector2 v1(-3.0f, 4.0f);
		ASSERT_NEAR_FLOAT(5.0f, v1.Length());
		ASSERT_NEAR_FLOAT(25.0f, v1.LengthSquared());
	}

	// Dot
	{
		Vector2 v1(1.0f, 2.0f);
		Vector2 v2(2.0f, 4.0f);

		ASSERT_NEAR_FLOAT(10.0f, v1.Dot(v2));
	}

	// Lerp
	{
		Vector2 v1(0.0f, 0.0f);
		Vector2 v2(30.0f, -60.0f);
		
		ASSERT_NEAR_VEC2(15.0f, -30.0f, Vector2::Lerp(v1, v2, 0.5f));
		ASSERT_NEAR_VEC2(10.0f, -20.0f, Vector2::Lerp(v1, v2, 1.0f / 3.0f));
	}

	// 演算子
	{
		Vector2 v1(1.0f, 2.0f);
		Vector2 v2(1.0f, 2.0f);
		ASSERT_EQ(true, v1 == v2);
		ASSERT_EQ(false, v1 != v2);
	}
	// 加算
	{
		Vector2 v1(2.0f, 3.0f);
		Vector2 sum = v1 + v1;
		ASSERT_NEAR_VEC2(4.0f, 6.0f, sum);
		v1 += v1;
		ASSERT_NEAR_VEC2(4.0f, 6.0f, v1);
	}
	// 乗算
	{
		Vector2 v1(2.0f, 3.0f);
		Vector2 mul = v1 * v1;
		ASSERT_NEAR_VEC2(4.0f, 9.0f, mul);
		v1 *= v1;
		ASSERT_NEAR_VEC2(4.0f, 9.0f, v1);
	}
	// 除算
	{
		Vector2 v1(2.0f, 3.0f);
		Vector2 mul = v1 / v1;
		ASSERT_NEAR_VEC2(1.0f, 1.0f, mul);
		v1 /= v1;
		ASSERT_NEAR_VEC2(1.0f, 1.0f, v1);
	}
	// 減算
	{
		Vector2 v1(2.0f, 3.0f);
		Vector2 mul = v1 - v1;
		ASSERT_NEAR_VEC2(0.0f, 0.0f, mul);
		v1 -= v1;
		ASSERT_NEAR_VEC2(0.0f, 0.0f, v1);
	}
}

} // namespace Math