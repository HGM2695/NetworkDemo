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

		using PacketHandler = std::function<void(PacketView)>;

	public:
		TcpSession(TcpSocket socket, PacketHandler packetHandler);

		PollResult		Poll();
		bool			Send(std::uint16_t packetId, std::span<const std::byte> payload);

	private:
		PollResult		TrySend();
		PollResult		TryReceive();
		PollResult		HandleReceivePackets();
	
	private:
		static constexpr std::size_t MaxBufferSize = 64 * 1024;

		TcpSocket		_socket{};
		ByteBuffer		_sendBuffer{ MaxBufferSize };
		ByteBuffer		_receiveBuffer{ MaxBufferSize };
		PacketHandler	_packetHandler{};
	};
}
