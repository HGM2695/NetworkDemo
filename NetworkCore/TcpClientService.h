#pragma once

#include "TcpConnector.h"
#include "TcpSession.h"
#include "Ipv4Endpoint.h"

#include <optional>
#include <functional>

namespace gm
{
	class TcpClientService
	{
	public:
		enum class State
		{
			Idle,
			Connecting,
			Connected,
		};

	public:
		bool	Initialize(TcpSession::PacketHandler packetHandler, TcpSession::SessionCloseHandler closeHandler);
		void	Tick();

		bool	Send(std::uint16_t packetId, std::span<const std::byte> payload);
		bool	Connect(const Ipv4Endpoint& endpoint);
		void	Disconnect();

		State	GetState() const { return _state; }

	private:
		void	CloseSession(TcpSession::DisconnectReason reason);
		
	private:
		State						_state = State::Idle;

		TcpConnector						_connector{};
		std::optional<TcpSession>			_session;
		TcpSession::PacketHandler			_packetHandler{};
		TcpSession::SessionCloseHandler		_sessionCloseHandler{};
	};
}
