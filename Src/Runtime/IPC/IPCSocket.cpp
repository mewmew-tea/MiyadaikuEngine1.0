#include "IPCSocket.h"
#include <cstdio>

namespace Miyadaiku
{
IPCSocket::IPCSocket()
{
}
IPCSocket::~IPCSocket()
{
	closesocket(m_sockHandle);
	WSACleanup();
}

bool IPCSocket::SetUp()
{
	// Initialize Winsock
	WSAData wsaData;

#pragma warning(push)
#pragma warning(disable : 6031)
	WSAStartup(MAKEWORD(1, 1), &wsaData);
#pragma warning(pop)

	// force IPV4
	addr.sin_family = AF_INET;
	// port number
	addr.sin_port = htons(59740);
	// IP address of client allowed connection
	addr.sin_addr.s_addr =
		htonl(INADDR_ANY);

	// create socket
	sockListen = socket(addr.sin_family, SOCK_STREAM, 0);

	if (sockListen == INVALID_SOCKET)
	{
		printf("socket creation failed.\n");
		return false;
	}

	// Alloc address to socket
	if (bind(sockListen, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR)
	{
		printf("socket bind failed.\n");
		return false;
	}

	// listen
	if (listen(sockListen, 5) == SOCKET_ERROR)
	{
		printf("socke listen failed.\n");
		return false;
	}

	return true;
}

void IPCSocket::WaitForClientConnection()
{
	m_sockHandle = accept(sockListen, (struct sockaddr*)&client, &clientlen);
	printf("accepted connection: %s (port=%d)\n", inet_ntoa(client.sin_addr),
		   ntohs(client.sin_port));
}

int32_t IPCSocket::Send(void* _data, int32_t _size)
{
	return send(m_sockHandle, (const char*)_data, _size, 0);
}

int32_t IPCSocket::SendResponse(std::string_view _commandStr)
{
	uint32_t length = _commandStr.length();
	Send(&length, sizeof(length));
	return Send((void*)_commandStr.data(), length);
}

int32_t IPCSocket::Recv(void* _buffer, int32_t _size)
{
	return recv(m_sockHandle, (char*)_buffer, _size, 0);
}

std::string IPCSocket::RecvCommand()
{
	uint32_t messageLength;
	Recv(&messageLength, sizeof(messageLength));
	//Send(&messageLength, sizeof(messageLength));

	char* data = (char*)malloc(messageLength);
	Recv(data, messageLength);
	std::string commandStr = "";
	if (data != nullptr)
	{
		commandStr = std::string(data, messageLength);
	}
	free(data);

	return commandStr;
}

void IPCSocket::Close()
{
	closesocket(m_sockHandle);
}

} // namespace Miyadaiku

