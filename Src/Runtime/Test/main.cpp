#include <gtest/gtest.h>
#include <iostream>
#include "TestEnvironment.h"

int main(int argc, char** argv)
{
	TestEnviroment::Setup();

	testing::InitGoogleTest(&argc, argv);
	int result = RUN_ALL_TESTS();

	TestEnviroment::Teardown();

	return result;
}
