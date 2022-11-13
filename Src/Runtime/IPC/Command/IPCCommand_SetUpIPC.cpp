#include "IPCCommand_SetUpIPC.h"
#include "../IPC.h"
#include "../../Core/Engine.h"

namespace Miyadaiku
{
std::string IPCCommand_SetUpIPC::Execute()
{
	Engine* pEngine = GetEngine();

	pEngine->m_editorHWnd = (void*)(uint64_t)m_data["hWnd"];
	pEngine->m_editorHeight = (int)m_data["height"];
	pEngine->m_editorWidth = (int)m_data["width"];

	// Response
	nlohmann::ordered_json response;
	response["result"] = true;
	return response.dump();
}
} // namespace Miyadaiku

