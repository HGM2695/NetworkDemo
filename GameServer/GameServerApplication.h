#pragma once

#include "GMEngine/TimeSystem.h"

#include "NetworkCore/TcpServerService.h"
#include "NetworkCore/WinsockRuntime.h"

#include "GameProtocol/GameProtocolTypes.h"

namespace gm
{
	class GameServerApplication
	{
	public:
		bool	Initialize();
		void	Run();

	private:
		void	HandlePacket(TcpSession::SessionId id, PacketView packet);

	private:
		WinsockRuntime		_winsockRuntime;
		TimeSystem			_timeSystem;
		TcpServerService	_serverService;

		std::uint16_t		_port = 49900;
		PlayerId			_nextPlayerId = 1;
	};
}
