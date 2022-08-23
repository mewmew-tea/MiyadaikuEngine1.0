#include "Engine.h"

#include "Platform/Platform.h"
#include "Platform/Window.h"
#include "Subsystem.h"

#include <iostream>
#include <assert.h>

#ifdef _WIN32
#include "Platform/Windows/Window_Windows.h"
#endif

namespace Miyadaiku
{
Engine* gEngine = nullptr;
Engine* GetEngine()
{
	return gEngine;
}

Engine::Engine()
{
	if (gEngine)
	{
		assert("エンジンのインスタンスが複数定義されています。");
	}
	gEngine = this;
	std::cout << "I am Miyadaiku Engine 1.0!" << std::endl;
}

Engine::~Engine()
{
}

void Engine::SetUp()
{

#ifdef _WIN32
	m_subsystemLocator.Add<Platform>();
#endif
}

void Engine::Update()
{
	auto os = m_subsystemLocator.Get<Platform>();
	os->ProcessSystemEventQueue();
}

void Engine::RequestShutdown()
{
	m_requestedShutdown = true;
}
} // namespace Miyadaiku