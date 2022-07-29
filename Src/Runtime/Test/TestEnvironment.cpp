#include "TestEnvironment.h"
#include "../Core/Engine.h"

void TestEnviroment::SetUp()
{
	m_upEngine = std::make_unique<Miyadaiku::Engine>();
}

void TestEnviroment::TearDown()
{
	m_upEngine.reset();
}
