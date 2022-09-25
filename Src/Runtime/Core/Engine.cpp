#include "Engine.h"
#include "Subsystem.h"

#include "Platform/Platform.h"
#include "Platform/Window.h"

#include "../Rendering/Renderer.h"
#include "../Rendering/RenderingResource.h"

#include "../Scripting/Scripting.h"

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
	m_subsystemLocator.Remove<Renderer>();
	m_subsystemLocator.Remove<Platform>();
	m_subsystemLocator.Remove<Scripting>();
}

void Engine::SetUp()
{
	m_subsystemLocator.Add<Platform>();
	m_subsystemLocator.Add<Renderer>();
	m_subsystemLocator.Add<Scripting>();
}

void Engine::Update()
{
	m_subsystemLocator.Get<Scripting>()->Update();

	auto os = m_subsystemLocator.Get<Platform>();
	os->ProcessSystemEventQueue();

	auto renderer = m_subsystemLocator.Get<Renderer>();
	renderer->Present();
}

void Engine::RequestShutdown()
{
	m_requestedShutdown = true;
}
} // namespace Miyadaiku