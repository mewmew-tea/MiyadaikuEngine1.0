#include "../Core/Math/Quaternion.h"
#include "TestUtils_Math.h"
#include "TestCommon.h"

using namespace Miyadaiku;

namespace Math
{

class Quaternion_Test : public ::testing::Test
{
protected:
	virtual void SetUp() override
	{
	}
	virtual void TearDown() override
	{
	}
};

TEST_F(Quaternion_Test, Calculation)
{
	// コンストラクタ
	{
		Quaternion q1;
		ASSERT_NEAR_VEC4(0.0f, 0.0f, 0.0f, 1.0f, q1);
		// 引数付き
		Quaternion q2(1.0f, 2.0f, 3.0f, 4.0f);
		ASSERT_NEAR_VEC4(1.0f, 2.0f, 3.0f, 4.0f, q2);
	}

	// Invert
	{
		Quaternion q1 = Quaternion::CreateFromYawPitchRoll(0.2f, 0.5f, 1.2f);
		Quaternion q2 = q1.Invert();

		Quaternion q3 = q1 * q2;
		ASSERT_NEAR_VEC4(0.0f, 0.0f, 0.0f, 1.0f, q3);
	}

	// EulerAngles
	{
		Quaternion q1 = Quaternion::CreateFromEulerAngles(1.0f, 2.0f, 3.0f);
		Vector3	   angles = q1.ToEulerAngles();

		ASSERT_NEAR_VEC3(1.0f, 2.0f, 3.0f, angles);
	}
}

} // namespace Math