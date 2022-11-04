#include "Platform.h"
#include "Windows/Window_Windows.h"
#include "../Engine.h"

namespace Miyadaiku
{
void Platform::OnAwake()
{
#ifdef _WIN32
	m_mainWindow = std::make_shared<Window_Windows>();
#endif
	if (m_mainWindow)
	{
		m_mainWindow->Create();
	}
}
void Platform::OnShutdown()
{
	if (m_mainWindow)
	{
		m_mainWindow->Destroy();
	}
}
void Platform::ProcessSystemEventQueue()
{
	Engine* e = GetEngine();
	if (!m_mainWindow || GetEngine()->m_editorHWnd)
	{
		return;
	}

	m_mainWindow->ProcessSystemEventQueue();
}

} // namespace Miyadaiku
