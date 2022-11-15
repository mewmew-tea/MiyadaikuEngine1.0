#pragma once
#include "IPCCommand.h"

namespace Miyadaiku
{

class IPCCommand_GetComponents : public IPCCommand
{
public:
	IPCCommand_GetComponents(const nlohmann::ordered_json& _data)
		: IPCCommand(_data){};
	std::string Execute() override;
};
}