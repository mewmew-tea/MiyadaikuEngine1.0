#pragma once
#include <memory>

namespace Miyadaiku
{
class Engine;
}

class Application;

class TestEnviroment
{
public:
	TestEnviroment();
	~TestEnviroment();

	static int	Setup();
	static void Teardown();

	static std::unique_ptr<Application> m_upApplication;
};
