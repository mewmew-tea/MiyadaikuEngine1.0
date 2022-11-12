#include "TestEnvironment.h"
#include "../Core/Application.h"
#include "../Core/Engine.h"

std::unique_ptr<Application> TestEnviroment::m_upApplication;

TestEnviroment::TestEnviroment()
{
}

TestEnviroment::~TestEnviroment()
{
	Teardown();
}

int TestEnviroment::Setup()
{
	m_upApplication = std::make_unique<Application>();
	if (m_upApplication)
		m_upApplication->Setup(false);

	return 0;
}

void TestEnviroment::Teardown()
{
	if (m_upApplication)
	{
		m_upApplication->Cleanup();
		m_upApplication.reset();
	}
}
