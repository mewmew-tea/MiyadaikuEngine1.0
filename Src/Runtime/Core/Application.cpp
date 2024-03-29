﻿//================================================
// Includes
//================================================
#include "Application.h"
#include "Engine.h"

//================================================

//================================================

Application::Application()
{

}

Application::~Application()
{
	Cleanup();
}

int Application::Setup(bool _isEnableStartUpAsEditor)
{
	m_engine = std::make_unique<Miyadaiku::Engine>();
	if (!m_engine)
	{
		return -1;
	}
	
	if (_isEnableStartUpAsEditor)
	{
		m_engine->EnableStartUpAsEditor();
	}
	m_engine->SetUp();
	return 0;
}

void Application::Iteration()
{
	while (!m_engine->IsRequestedShutdown())
	{
		m_engine->Update();
	}
}

void Application::Cleanup()
{
	if (m_engine.get())
	{
		m_engine.reset();
	}
}
