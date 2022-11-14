#include "IPCCommand_SetUpIPC.h"
#include "../IPC.h"
#include "../../Core/Engine.h"
#include "../../Core/Platform/Windows/Window_Windows.h"
#include "../../Core/Platform/Platform.h"

namespace Miyadaiku
{
std::string IPCCommand_SetUpIPC::Execute()
{
	Engine* pEngine = GetEngine();

	std::shared_ptr<Window_Windows> window =
		static_pointer_cast<Window_Windows>(GetEngine()
												->GetSubsystemLocator()
												.Get<Platform>()
												->GetMainWindow());
	window->SetParentWindowHandle((HWND)(uint64_t)m_data["hWnd"]);
	window->Show();

	// Response
	nlohmann::ordered_json response;
	response["result"] = true;
	response["hWnd"] = (uint64_t)window->GetWindowHandle();
	return response.dump();
}
} // namespace Miyadaiku

