#include "../Core/Math/Matrix.h"
#include "../Core/Math/Vector3.h"
#include "TestUtils_Math.h"
#include "TestCommon.h"

using namespace Miyadaiku;

namespace Math
{

class Matrix_Test : public ::testing::Test
{
protected:
	virtual void SetUp() override
	{
	}
	virtual void TearDown() override
	{
	}
};

TEST_F(Matrix_Test, Calculation)
{
	{
		Matrix m1;
		m1.m41 = 10;
		m1.m42 = 20;
		m1.m43 = 30;
		Matrix m2;
		m2.m41 = 30;
		m2.m42 = 40;
		m2.m43 = 50;

		m1 *= m2;

		Vector3 v = m1.GetTranslation();
		ASSERT_NEAR_VEC3(40.0f, 60.0f, 80.0f, v);
	}

	// コンストラクタ
	{
		Matrix m1;
		ASSERT_NEAR_MAT(1.0f, 0.0f, 0.0f, 0.0f, //
						0.0f, 1.0f, 0.0f, 0.0f, //
						0.0f, 0.0f, 1.0f, 0.0f, //
						0.0f, 0.0f, 0.0f, 1.0f, m1);
		// 引数付き
		Matrix m2(1.0f, 0.0f, 0.0f, 0.0f, //
				  0.0f, 1.0f, 0.0f, 0.0f, //
				  0.0f, 0.0f, 1.0f, 0.0f, //
				  3.0f, 4.0f, 6.0f, 1.0f);
		ASSERT_NEAR_MAT(1.0f, 0.0f, 0.0f, 0.0f, //
						0.0f, 1.0f, 0.0f, 0.0f, //
						0.0f, 0.0f, 1.0f, 0.0f, //
						3.0f, 4.0f, 6.0f, 1.0f, m2);
	}

	// CreateScale
	{
		auto m = Matrix::CreateScale(1, -2, 3);
		auto s = m.GetScale();
		ASSERT_NEAR_MAT(1.0f, 0.0f, 0.0f, 0.0f,	 //
						0.0f, -2.0f, 0.0f, 0.0f, //
						0.0f, 0.0f, 3.0f, 0.0f,	 //
						0.0f, 0.0f, 0.0f, 1.0f, m);
		ASSERT_NEAR_VEC3(1, 2, 3, s);
	}

	// Invert
	{
		Matrix m1 = Matrix::CreateScale(1, -2, 3) * Matrix::CreateTranslation(3,4,5);
		Matrix m2 = m1.Invert();

		Matrix m3 = m1 * m2;
		ASSERT_NEAR_MAT(1.0f, 0.0f, 0.0f, 0.0f, //
						0.0f, 1.0f, 0.0f, 0.0f, //
						0.0f, 0.0f, 1.0f, 0.0f, //
						0.0f, 0.0f, 0.0f, 1.0f, m3);
	}


}

} // namespace Math