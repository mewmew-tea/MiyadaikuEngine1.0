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
	// ”O‚Ì‚½‚ß‚ÉA–¾¦“I‚É‰ğ•ú‚µ‚Ä‚¨‚­
	if (m_engine.get())
	{
		m_engine.release();
	}
}