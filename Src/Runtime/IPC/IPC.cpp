#include "IPC.h"
#include "Command/IPCCommand_SetUpIPC.h"
#include "../Core/Engine.h"

namespace Miyadaiku
{
#define REGISTER_COMMAND(name, T)                                              \
	m_commandNameToGenerateInstanceFunction[name] =                            \
		[](const nlohmann::ordered_json& j)                                    \
	{                                                                          \
		return std::make_unique<T>(j);                                         \
	}

bool IPC::SetUp()
{
	REGISTER_COMMAND("SetUpIPC", IPCCommand_SetUpIPC);

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
	// Recv command
	auto recvStr = m_upSocket->RecvCommand();
	printf("%s\n", recvStr.c_str());
	auto commandJson = nlohmann::ordered_json::parse(recvStr);
	auto upCommand = CreateCommand(commandJson["commandID"], commandJson);
	// Execute
	std::string responseStr = upCommand->Execute();
	// Send response
	m_upSocket->SendResponse(responseStr);
}

void IPC::Disconnect()
{
	m_upSocket->Close();
}

std::unique_ptr<IPCCommand>
IPC::CreateCommand(const std::string&			 _commandID,
				   const nlohmann::ordered_json& _data) const
{
	if (m_commandNameToGenerateInstanceFunction.find(_commandID)
		== m_commandNameToGenerateInstanceFunction.end())
		return nullptr;

	return (m_commandNameToGenerateInstanceFunction.find(_commandID)->second)(
		_data);
}

} // namespace Miyadaiku

