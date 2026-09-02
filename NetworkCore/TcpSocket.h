#pragma once

#include <WinSock2.h>

namespace gm
{
	class TcpSocket
	{
	public:
		TcpSocket() = default;
		~TcpSocket();

		TcpSocket(const TcpSocket&) = delete;
		TcpSocket& operator=(const TcpSocket&) = delete;

		TcpSocket(TcpSocket&& other);
		TcpSocket& operator=(TcpSocket&& other);

		bool	Open();
		void	Close();

		bool	IsValid() const;
		SOCKET	GetNativeSocket() const;
		bool	SetNativeSocket(SOCKET socket);

	private:
		bool	SetNonBlocking();

	private:
		SOCKET _socket = INVALID_SOCKET;
	};
}