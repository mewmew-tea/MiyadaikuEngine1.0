#include "IPC.h"
#include "../Core/Engine.h"

namespace Miyadaiku
{

struct EditorData
{
	HWND hwnd;
	int	 width;
	int	 height;
};

bool IPC::SetUp()
{
	m_upSocket = std::make_unique<IPCSocket>();
	if (!m_upSocket->SetUp())
	{
		return false;
	}
	m_upSocket->WaitForClientConnection();
	ProcessCommands();
	return true;
}

void IPC::ProcessCommands()
{
	char buf[256];
	memset(buf, 0, sizeof(buf));
	m_upSocket->Recv(buf, sizeof(buf));

	EditorData* data = (EditorData*)buf;
	printf("resolution : %d, %d\n", data->width, data->height);
	//
	// 
	
	Engine* pEngine = GetEngine();
	// 
	if (!(pEngine->m_editorHWnd))
	{
		pEngine->m_editorHWnd = data->hwnd;
		pEngine->m_editorHeight = data->height;
		pEngine->m_editorWidth = data->width;
	}
	else
	{

		//auto platform =
		//	pEngine->GetSubsystemLocator().Get<Miyadaiku::Platform>();
		//auto hWnd = static_pointer_cast<Miyadaiku::Window_Windows>(
		//				platform->GetMainWindow())
		//				->GetWindowHandle();
	}
	//
	char sendBuf[256] = "This is Engine.";
	// memcpy(sendBuf, &hWnd, sizeof(hWnd));
	//memcpy(sendBuf, &hWnd, sizeof(sendBuf));
	m_upSocket->Send(sendBuf, sizeof(char) * 256);
}

void IPC::Disconnect()
{
	m_upSocket->Close();
}

} // namespace Miyadaiku

