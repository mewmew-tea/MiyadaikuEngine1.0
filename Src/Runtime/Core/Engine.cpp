#include "Engine.h"
#include "Subsystem.h"

#include <stdio.h>

#include "Platform/Platform.h"
#include "Platform/Window.h"

#include "Timer/Timer.h"

#include "../Rendering/Renderer.h"
#include "../Rendering/RenderingResource.h"

#include "../Scripting/Scripting.h"


#include "../IPC/IPC.h"

#include <iostream>
#include <assert.h>

#ifdef _WIN32
#include "Platform/Windows/Window_Windows.h"
#endif

#include <iostream>
#include <string>


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
	if (IsEditorMode())
	{
		m_subsystemLocator.Remove<IPC>();
	}
	m_subsystemLocator.Remove<Renderer>();
	m_subsystemLocator.Remove<Platform>();
	m_subsystemLocator.Remove<Scripting>();
	m_subsystemLocator.Remove<Timer>();
}

void Engine::SetUp()
{
	m_subsystemLocator.Add<Timer>();
	m_subsystemLocator.Add<Platform>();
	m_subsystemLocator.Add<Renderer>();
	m_subsystemLocator.Add<Scripting>();
	if (IsEditorMode())
	{
		m_subsystemLocator.Add<IPC>();
	}
}

void Engine::Update()
{
	if (IsEditorMode())
	{
		m_subsystemLocator.Get<IPC>()->ProcessCommands();
	}

	m_subsystemLocator.Get<Scripting>()->Update();

	auto os = m_subsystemLocator.Get<Platform>();
	os->ProcessSystemEventQueue();

	auto renderer = m_subsystemLocator.Get<Renderer>();
	renderer->Present();

	
	auto timer = m_subsystemLocator.Get<Timer>();
	timer->Tick();
}

void Engine::RequestShutdown()
{
	m_requestedShutdown = true;
}
void Engine::EnableStartUpAsEditor()
{
	m_isEditorMode = true;
}
bool Engine::IsEditorMode()
{
	return m_isEditorMode;
}
} // namespace Miyadaiku