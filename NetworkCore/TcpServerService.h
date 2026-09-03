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
		bool	Initialize();
		void	Tick();

	private:
		void					CheckAccept();
		TcpSession::SessionId	GetNextSessionId();

	private:
		TcpListener				_listener;
		Ipv4Endpoint			_endpoint{};
		std::uint16_t			_port = 49900;

		std::vector<TcpSession>		_sessionList;
		TcpSession::SessionId		_nextSessionId = 1;
		TcpSession::PacketHandler	_sessionPacketHandler{};
	};
}
