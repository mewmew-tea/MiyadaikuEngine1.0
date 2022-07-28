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
	// �O�̂��߂ɁA�����I�ɉ�����Ă���
	if (m_engine.get())
	{
		m_engine.release();
	}
}