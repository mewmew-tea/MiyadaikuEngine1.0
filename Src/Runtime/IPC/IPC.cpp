#include "IPC.h"
#include "Command/IPCCommand_SetUpIPC.h"
#include "../Core/Engine.h"

namespace Miyadaiku
{
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
	// Recv command
	auto				recvStr = m_upSocket->RecvCommand();
	printf("%s\n", recvStr.c_str());
	auto				commandJson = nlohmann::ordered_json::parse(recvStr);
	IPCCommand_SetUpIPC command(commandJson);
	// Execute
	std::string			responseStr = command.Execute();
	// Send response
	m_upSocket->SendResponse(responseStr);
}

void IPC::Disconnect()
{
	m_upSocket->Close();
}

} // namespace Miyadaiku

