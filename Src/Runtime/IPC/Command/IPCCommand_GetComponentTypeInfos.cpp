#include "IPCCommand_GetComponentTypeInfos.h"
#include "../../Scripting/Scripting.h"
#include "../../Core/Engine.h"

namespace Miyadaiku
{
std::string IPCCommand_GetComponentTypeInfos::Execute()
{
	Engine* pEngine = GetEngine();

	// Response
	nlohmann::ordered_json	   response;
	std::shared_ptr<Scripting> spScripting =
		GetEngine()->GetSubsystemLocator().Get<Scripting>();
	nlohmann::ordered_json typeInfos;
	for (auto& spTypeInfo : spScripting->GetScriptClassTypes())
	{
		nlohmann::ordered_json typeInfo;   
		spTypeInfo->Serialize(typeInfo);
		typeInfos.push_back(typeInfo);
	}

	response["typeInfos"] = typeInfos;
	return response.dump();
}
} // namespace Miyadaiku