//================================================
// Includes
//================================================
#include "Application.h"
#include "../Runtime/Core/Engine.h"

//================================================

//================================================

Application::Application()
{
	m_engine = std::make_unique<Miyadaiku::Engine>();
}

Application::~Application()
{
	// 念のために、明示的に解放しておく
	if (m_engine.get())
	{
		m_engine.release();
	}
}