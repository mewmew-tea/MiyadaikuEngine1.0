#pragma once
#include "IPCCommand.h"

namespace Miyadaiku
{

class IPCCommand_GetComponentTypeInfos : public IPCCommand
{
public:
	IPCCommand_GetComponentTypeInfos(const nlohmann::ordered_json& _data)
		: IPCCommand(_data){};
	std::string Execute() override;
};
}