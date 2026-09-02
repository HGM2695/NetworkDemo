#pragma once

#include <WinSock2.h>
#include <span>

namespace gm
{
	class TcpSocket
	{
	public:
		enum class IoStatus
		{
			Transferred,
			WouldBlock,
			Failed,
			Closed,
		};

		struct IoResult
		{
			IoStatus	status;
			int			transferredBytes = 0;
		};

	public:
		TcpSocket() = default;
		~TcpSocket();

		TcpSocket(const TcpSocket&) = delete;
		TcpSocket& operator=(const TcpSocket&) = delete;

		TcpSocket(TcpSocket&& other);
		TcpSocket& operator=(TcpSocket&& other);

		bool		Open();
		void		Close();

		IoResult	TrySend(std::span<const std::byte> buffer);
		IoResult	TryReceive(std::span<std::byte> buffer);

		bool		IsValid() const;
		SOCKET		GetNativeSocket() const;
		bool		SetNativeSocket(SOCKET socket);

	private:
		bool		SetNonBlocking();
		IoResult	HandleIoError();

	private:
		SOCKET _socket = INVALID_SOCKET;
	};
}