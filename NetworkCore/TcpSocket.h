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

		bool	Open();
		void	Close();

		bool	IsValid() const;
		SOCKET	GetNativeSocket() const;

	private:
		SOCKET _socket = INVALID_SOCKET;
	};
}