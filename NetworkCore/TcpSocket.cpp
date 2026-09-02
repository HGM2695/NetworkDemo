#include "TcpSocket.h"

namespace gm
{
	TcpSocket::~TcpSocket()
	{
		Close();
	}

	bool TcpSocket::Open()
	{
		if (IsValid())
			return true;

		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_socket == INVALID_SOCKET)
			return false;

		u_long mode = 1;
		if (ioctlsocket(_socket, FIONBIO, &mode) != NO_ERROR)
		{
			Close();
			return false;
		}

		return true;
	}

	void TcpSocket::Close()
	{
		if (IsValid())
			closesocket(_socket);

		_socket = INVALID_SOCKET;
	}

	bool TcpSocket::IsValid() const
	{
		return _socket != INVALID_SOCKET;
	}

	SOCKET TcpSocket::GetNativeSocket() const
	{
		return _socket;
	}
}
