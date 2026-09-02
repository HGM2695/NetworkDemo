#include "TcpSocket.h"

namespace gm
{
	TcpSocket::~TcpSocket()
	{
		Close();
	}

	TcpSocket::TcpSocket(TcpSocket&& other) : _socket(other._socket)
	{
		other._socket = INVALID_SOCKET;
	}

	TcpSocket& TcpSocket::operator=(TcpSocket&& other)
	{
		if (this == &other)
			return *this;

		Close();

		_socket = other._socket;
		other._socket = INVALID_SOCKET;
		return *this;
	}

	bool TcpSocket::Open()
	{
		if (IsValid())
			return true;

		_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (_socket == INVALID_SOCKET)
			return false;

		if (SetNonBlocking() == false)
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

	bool TcpSocket::SetNativeSocket(SOCKET socket)
	{
		if (socket == INVALID_SOCKET)
			return false;

		if (socket == _socket)
			return true;

		if (IsValid())
			Close();

		_socket = socket;

		if (SetNonBlocking() == false)
		{
			Close();
			return false;
		}

		return true;
	}

	bool TcpSocket::SetNonBlocking()
	{
		if (IsValid() == false)
			return false;

		u_long mode = 1;
		if (ioctlsocket(_socket, FIONBIO, &mode) != NO_ERROR)
			return false;

		return true;
	}
}
