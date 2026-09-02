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

	TcpSocket::IoResult TcpSocket::TrySend(std::span<const std::byte> buffer)
	{
		if (IsValid() == false)
			return IoResult{ IoStatus::Failed, 0 };

		if (buffer.size() > static_cast<std::size_t>(INT_MAX))
			return IoResult{ IoStatus::Failed, 0 };

		const int result = send(_socket, reinterpret_cast<const char*>(buffer.data()), static_cast<int>(buffer.size()), 0);
		if (result == SOCKET_ERROR)
			return HandleIoError();

		return IoResult{ IoStatus::Transferred, result };
	}

	TcpSocket::IoResult TcpSocket::TryReceive(std::span<std::byte> buffer)
	{
		if (IsValid() == false)
			return IoResult{ IoStatus::Failed, 0 };

		if (buffer.empty())
			return { IoStatus::Failed, 0 };

		if (buffer.size() > static_cast<std::size_t>(INT_MAX))
			return IoResult{ IoStatus::Failed, 0 };

		const int result = recv(_socket, reinterpret_cast<char*>(buffer.data()), static_cast<int>(buffer.size()), 0);
		if (result == SOCKET_ERROR)
			return HandleIoError();

		if (result == 0)
			return { IoStatus::Closed, 0 };

		return IoResult{ IoStatus::Transferred, result };
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

	TcpSocket::IoResult TcpSocket::HandleIoError()
	{
		const int err = WSAGetLastError();
		if (err == WSAEWOULDBLOCK)
			return IoResult{ IoStatus::WouldBlock, 0 };

		return IoResult{ IoStatus::Failed, 0 };
	}
}
