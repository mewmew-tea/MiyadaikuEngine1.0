#include "IPC.h"
#include "Command/IPCCommand_SetUpIPC.h"
#include "Command/IPCCommand_GetComponentTypeInfos.h"
#include "../Core/Engine.h"

namespace Miyadaiku
{
#define REGISTER_COMMAND(name, T)                                              \
	m_commandNameToGenerateInstanceFunction[name] =                            \
		[](const nlohmann::ordered_json& j)                                    \
	{                                                                          \
		return std::make_shared<T>(j);                                         \
	}

bool IPC::SetUp()
{
	REGISTER_COMMAND("SetUpIPC", IPCCommand_SetUpIPC);
	REGISTER_COMMAND("GetComponentTypeInfos", IPCCommand_GetComponentTypeInfos);

	m_upSocket = std::make_unique<IPCSocket>();
	if (!m_upSocket->SetUp())
	{
		return false;
	}
	m_upSocket->WaitForClientConnection();

	// Begin threads
	m_recvThread = std::thread(
		[this]()
		{
			RecvThread();
		});
	m_responseThread = std::thread(
		[this]()
		{
			ResponseThread();
		});

	{
	while (true)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		if (m_commandQueue.size() > 0)
		{
			break;
		}
	}
	}

	ProcessCommands();
	return true;
}

void IPC::ProcessCommands()
{
	std::queue<std::shared_ptr<IPCCommand>> commandQueueCopy;

	{
		std::lock_guard<std::mutex> lock(m_mutex);

		while (!m_commandQueue.empty())
		{
			commandQueueCopy.emplace(m_commandQueue.front());
			m_commandQueue.pop();
		}
	}

	while (!commandQueueCopy.empty())
	{
		m_responseQueue.push(commandQueueCopy.front()->Execute());
		commandQueueCopy.pop();
	}
}

void IPC::Disconnect()
{
	// join threads
	if (m_recvThread.joinable())
	{
		m_recvThread.join();
	}
	if (m_responseThread.joinable())
	{
		m_responseThread.join();
	}

	m_upSocket->Close();
}

void IPC::RecvThread()
{
	while (true)
	{
		// Recv command
		auto recvStr = m_upSocket->RecvCommand();
		//printf("%s\n", recvStr.c_str());
		auto commandJson = nlohmann::ordered_json::parse(recvStr);
		auto spCommand = CreateCommand(commandJson["commandID"], commandJson);

		std::lock_guard<std::mutex> lock(m_mutex);
		m_commandQueue.push(spCommand);
	}

}

void IPC::ResponseThread()
{
	while (true)
	{
		std::lock_guard<std::mutex> lock(m_mutex);

		if (m_responseQueue.empty())
			continue;

		m_upSocket->SendResponse(m_responseQueue.front());
		m_responseQueue.pop();
	}
}

std::shared_ptr<IPCCommand>
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

