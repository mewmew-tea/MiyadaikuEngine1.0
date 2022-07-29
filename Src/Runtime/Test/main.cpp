#include "TestEnvironment.h"
#include <gtest/gtest.h>
#include <iostream>

int main(int argc, char** argv)
{
	TestEnviroment::SetUp();

	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();
	
	TestEnviroment::TearDown();

	return result;
}
