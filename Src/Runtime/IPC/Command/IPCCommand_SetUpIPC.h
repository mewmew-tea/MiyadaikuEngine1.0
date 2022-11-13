#pragma once
#include "IPCCommand.h"

namespace Miyadaiku
{
class IPCCommand_SetUpIPC : public IPCCommand
{
public:
	IPCCommand_SetUpIPC(const nlohmann::ordered_json& _data) : IPCCommand(_data){};
	std::string Execute() override;
};
} // namespace Miyadaiku