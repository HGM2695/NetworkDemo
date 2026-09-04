#pragma once

#include "PacketTypes.h"
#include "TcpSocket.h"
#include "ByteBuffer.h"

#include <functional>

namespace gm
{
	class TcpSession
	{
	public:
		enum class PollResult
		{
			Alive,
			Closed,
			Failed,
			Invalid
		};

		using SessionId = std::uint32_t;
		static constexpr SessionId InvalidSessionId = 0;

		using PacketHandler = std::function<void(SessionId, PacketView)>;

	public:
		TcpSession(SessionId sessionId, TcpSocket socket, PacketHandler packetHandler);

		PollResult			Tick(bool readable, bool writable);
		bool				Send(std::uint16_t packetId, std::span<const std::byte> payload);
		const TcpSocket&	GetSocket() const;

		bool				GetPendingSend() const { return _pendingSend; }
		SessionId			GetSessionId() const { return _sessionId; }

	private:
		PollResult			TrySend();
		PollResult			TryReceive();
		PollResult			HandleReceivePackets();

	private:
		static constexpr std::size_t MaxBufferSize = 64 * 1024;

		SessionId		_sessionId = InvalidSessionId;
		TcpSocket		_socket{};
		ByteBuffer		_sendBuffer{ MaxBufferSize };
		ByteBuffer		_receiveBuffer{ MaxBufferSize };
		PacketHandler	_packetHandler{};

		bool			_pendingSend{};
	};
}
