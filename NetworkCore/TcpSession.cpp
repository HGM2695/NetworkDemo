#include "TcpSession.h"

#include <array>
#include <algorithm>
#include <cstring>
#include <utility>


namespace gm
{
	TcpSession::TcpSession(SessionId sessionId, TcpSocket socket, PacketHandler packetHandler) : _sessionId(sessionId), _socket(std::move(socket)), _packetHandler(std::move(packetHandler))
	{
	}

	TcpSession::PollResult TcpSession::Tick(bool readable, bool writable)
	{

		PollResult result = PollResult::Alive;
		if (readable)
		{
			result = TryReceive();
			if (result != PollResult::Alive)
				return result;

			result = HandleReceivePackets();
			if (result != PollResult::Alive)
				return result;
		}

		if (writable)
		{
			result = TrySend();
			if (result != PollResult::Alive)
				return result;
		}

		return PollResult::Alive;
	}

	bool TcpSession::Send(std::uint16_t packetId, std::span<const std::byte> payload)
	{
		const std::size_t packetSize = PacketHeaderSize + payload.size();

		if (packetSize > MaxPacketSize)
			return false;

		if (_sendBuffer.CanWrite(packetSize) == false)
			return false;

		PacketHeader header{};
		header.packetSize = static_cast<uint16_t>(packetSize);
		header.packetId = packetId;

		_sendBuffer.Write(std::as_bytes(std::span{ &header, 1 }));
		_sendBuffer.Write(payload);

		_pendingSend = true;
		return true;
	}

	const TcpSocket& TcpSession::GetSocket() const
	{
		return _socket;
	}

	TcpSession::PollResult TcpSession::TrySend()
	{
		if (_sendBuffer.Empty())
			return PollResult::Alive;

		const TcpSocket::IoResult ioResult = _socket.TrySend(_sendBuffer.Peek());
		switch (ioResult.status)
		{
		case TcpSocket::IoStatus::Transferred:
			_sendBuffer.Consume(ioResult.transferredBytes);
			break;

		case TcpSocket::IoStatus::WouldBlock:
			break;

		case TcpSocket::IoStatus::Failed:
			return PollResult::Failed;
		case TcpSocket::IoStatus::Closed:
			return PollResult::Closed;
		}

		if (_sendBuffer.Empty())
			_pendingSend = false;
		
		return PollResult::Alive;
	}

	TcpSession::PollResult TcpSession::TryReceive()
	{
		static constexpr std::size_t receiveChunkSize = 4096;

		const std::size_t writableSize = _receiveBuffer.GetWritableSize();
		if (writableSize == 0)
			return PollResult::Alive;

		std::array<std::byte, receiveChunkSize> buffer;
		const auto receiveSpan = std::span{ buffer }.first((std::min)(writableSize, receiveChunkSize));

		const TcpSocket::IoResult ioResult = _socket.TryReceive(receiveSpan);
		switch (ioResult.status)
		{
		case TcpSocket::IoStatus::Transferred:
			_receiveBuffer.Write(receiveSpan.first(ioResult.transferredBytes));
			break;

		case TcpSocket::IoStatus::WouldBlock:
			break;

		case TcpSocket::IoStatus::Failed:
			return PollResult::Failed;
		case TcpSocket::IoStatus::Closed:
			return PollResult::Closed;
		}

		return PollResult::Alive;
	}

	TcpSession::PollResult TcpSession::HandleReceivePackets()
	{
		if (_packetHandler == nullptr)
			return PollResult::Failed;

		while (_receiveBuffer.GetReadableSize() >= PacketHeaderSize)
		{
			std::span<const std::byte> bufferView = _receiveBuffer.Peek();

			PacketHeader header{};
			std::memcpy(&header, bufferView.data(), PacketHeaderSize);

			const std::size_t packetSize = header.packetSize;
			if (packetSize < PacketHeaderSize || packetSize > MaxPacketSize)
				return PollResult::Invalid;

			if (packetSize > _receiveBuffer.GetReadableSize())
				break;
			
			PacketView packetView{};
			packetView.header = header;
			packetView.payload = bufferView.subspan(PacketHeaderSize, packetSize - PacketHeaderSize);

			if (_sessionId == InvalidSessionId)
				return PollResult::Failed;

			_packetHandler(_sessionId, packetView);
			_receiveBuffer.Consume(packetSize);
		}

		return PollResult::Alive;
	}
}