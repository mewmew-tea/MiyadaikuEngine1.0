#include "../Core/Math/Vector4.h"
#include "TestUtils_Math.h"
#include "TestCommon.h"

using namespace Miyadaiku;

namespace Math
{

class Vector4_Test : public ::testing::Test
{
protected:
	virtual void SetUp() override
	{
	}
	virtual void TearDown() override
	{
	}
};

TEST_F(Vector4_Test, Calculation)
{
	// コンストラクタ
	{
		Vector4 v1;
		ASSERT_NEAR_VEC4(0.0f, 0.0f, 0.0f, 0.0f, v1);
		// 引数付き
		Vector4 v2(1.0f, 2.0f, 3.0f, 4.0f);
		ASSERT_NEAR_VEC4(1.0f, 2.0f, 3.0f, 4.0f, v2);
	}

}

} // namespace Math