#include <gtest/gtest.h>
#include "../Core/Math/Vector2I.h"

#define TEST_EQUAL_Vector2I(x, y, val) \
	ASSERT_EQ(x, val.x); \                                                       \
	ASSERT_EQ(y, val.y);
	
