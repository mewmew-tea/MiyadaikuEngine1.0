//================================================
// Includes
//================================================
#include "Application.h"
#include "../Runtime/Core/Engine.h"

//================================================

//================================================

Application::Application()
{

}

Application::~Application()
{
	Cleanup();
}

int Application::Setup()
{
	m_engine = std::make_unique<Miyadaiku::Engine>();

	return 0;
}

void Application::Cleanup()
{
	if (m_engine.get())
	{
		m_engine.reset();
	}
}
