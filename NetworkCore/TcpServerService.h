#pragma once

#include "TcpSession.h"
#include "TcpListener.h"

#include <cstdint>
#include <vector>

namespace gm
{
	class TcpServerService
	{
	public:
		bool					Initialize(std::uint16_t port, TcpSession::PacketHandler packetHandler);
		void					Tick();

		bool					Send(TcpSession::SessionId sessionId, std::uint16_t packetId, std::span<const std::byte> payload);
		void					Broadcast(std::uint16_t packetId, std::span<const std::byte> payload);

	private:
		void					CheckAccept();
		TcpSession::SessionId	GetNextSessionId();
		TcpSession*				FindSession(TcpSession::SessionId id);

	private:
		TcpListener					_listener;
		Ipv4Endpoint				_endpoint{};
		std::uint16_t				_port{};

		std::vector<TcpSession>		_sessionList;
		TcpSession::SessionId		_nextSessionId = 1;
		TcpSession::PacketHandler	_sessionPacketHandler{};
	};
}
