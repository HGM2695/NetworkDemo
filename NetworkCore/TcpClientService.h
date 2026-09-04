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

		using ClientPacketHandler = std::function<void(PacketView)>;

	public:
		bool	Initialize(ClientPacketHandler packetHandler);
		void	Tick();

		bool	Send(std::uint16_t packetId, std::span<const std::byte> payload);
		bool	Connect(const Ipv4Endpoint& endpoint);
		void	Disconnect();

		State	GetState() const { return _state; }
		
	private:
		State						_state = State::Idle;

		TcpConnector				_connector{};
		std::optional<TcpSession>	_session;
		ClientPacketHandler			_packetHandler{};
	};
}
