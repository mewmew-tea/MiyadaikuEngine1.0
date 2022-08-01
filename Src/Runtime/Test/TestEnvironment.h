#pragma once
#include <memory>
#include "../../Application/Application.h"

namespace Miyadaiku
{
class Engine;
}

//class Application;

class TestEnviroment
{
public:
	TestEnviroment();
	~TestEnviroment();

	static int	Setup();
	static void Teardown();

	static std::unique_ptr<Application> m_upApplication;
};
