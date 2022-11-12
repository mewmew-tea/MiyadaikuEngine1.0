#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCKAPI_
#define NOMINMAX
#include <winsock2.h>
#include <WS2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include <cstdint>
#include <string>

namespace Miyadaiku
{
#ifdef _WIN32
using SocketHandle = SOCKET;
using SockLen = int;
using InAddr = IN_ADDR;
using SockAddrIn = SOCKADDR_IN;
using SockAddr = SOCKADDR;

static const SocketHandle InvalidSockHandle = INVALID_SOCKET;
static const int32_t	SocketError = SOCKET_ERROR;
static const int32_t	InaddrNone = INADDR_NONE;
#endif // _WIN32

/**
 * TCPSocket for IPC.
 */
class IPCSocket
{
public:
	IPCSocket();
	~IPCSocket();

	bool SetUp();
	void WaitForClientConnection();

	int32_t Send(void* data, int32_t size);

	int32_t Recv(void* buffer, int32_t size);
	/**
	 * @brief Recieve command(header and data).
	 * 
	 * @return 
	 */
	//std::string RecvCommand();

	bool	IsValid() const
	{
		return m_sockHandle != InvalidSockHandle;
	}

	void Close();

private:
	SocketHandle	   m_sockHandle = InvalidSockHandle;
	struct sockaddr_in client = {};
	int				   clientlen = sizeof(client);

	SocketHandle sockListen = InvalidSockHandle;

	struct sockaddr_in addr = {};
};
}