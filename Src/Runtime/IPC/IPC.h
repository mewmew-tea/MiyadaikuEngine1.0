#pragma once
#include "../Core/Subsystem.h"
#include "IPCSocket.h"
#include <memory>
#include <nlohmann/json.hpp>
#include <functional>

namespace Miyadaiku
{
class IPCCommand;

/**
 * @brief Management for IPC(Inter-Process Communication) between runtime and editor.
 * Runtime acts as a server.
 */
class IPC final : public Subsystem
{
public:
	// Subsystem is added
	void OnAwake()
	{
		SetUp();
	}
	// Subsystem is removed
	void OnShutdown()
	{
		Disconnect();
	}

	/**
	 * @brief SetUp IPC System.
	 */
	bool SetUp();

	/**
	 * @brief Process recieved commands.
	 * 
	 */
	void ProcessCommands();


	void Disconnect();



private:
	std::unique_ptr<IPCSocket> m_upSocket;
	using FactoryFunction = std::function<std::unique_ptr<IPCCommand>(const nlohmann::ordered_json&)>;

	std::unordered_map<std::string, FactoryFunction> m_commandNameToGenerateInstanceFunction;

	std::unique_ptr<IPCCommand> CreateCommand(const std::string& _commandID,
				  const nlohmann::ordered_json& _data) const;
};
}