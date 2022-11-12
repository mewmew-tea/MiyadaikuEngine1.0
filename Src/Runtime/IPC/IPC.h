#pragma once
#include "../Core/Subsystem.h"
#include "IPCSocket.h"
#include <memory>

namespace Miyadaiku
{
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
};
}