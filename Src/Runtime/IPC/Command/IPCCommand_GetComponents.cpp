#include "IPCCommand_GetComponents.h"

#include "../../Scripting/Scripting.h"
#include "../../Scripting/Component.h"
#include "../../Core/Engine.h"

namespace Miyadaiku
{
std::string IPCCommand_GetComponents::Execute()
{
	Engine* pEngine = GetEngine();

	// Response
	nlohmann::ordered_json	   response;
	std::shared_ptr<Scripting> spScripting =
		GetEngine()->GetSubsystemLocator().Get<Scripting>();
	nlohmann::ordered_json components;

	// TODO: search gameobject
	for (auto& spGameObject : spScripting->GetGameObjects())
	{
		
		for (auto& spComponent : spGameObject->GetComponents())
		{
			nlohmann::ordered_json typeInfo;
			spComponent->GetClassInstance()->Serialize(typeInfo);
			components.push_back(typeInfo);
		}
		break;
	}

	response["components"] = components;
	return response.dump();
}
} // namespace Miyadaiku