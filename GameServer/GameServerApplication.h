#pragma once

#include "GMEngine/TimeSystem.h"
#include "GMEngine/SceneManager.h"
#include "GMEngine/PhysicsSystem2D.h"

#include "NetworkCore/TcpServerService.h"
#include "NetworkCore/WinsockRuntime.h"

#include "GameProtocol/GameProtocolTypes.h"

namespace gm
{
	class GameServerScene;

	class GameServerApplication
	{
	public:
		bool	Initialize();
		void	Run();

	private:
		void	HandlePacket(TcpSession::SessionId id, PacketView packet);

	private:
		WinsockRuntime		_winsockRuntime;
		TcpServerService	_serverService;

		std::uint16_t		_port = 49900;
		PlayerId			_nextPlayerId = 1;

		TimeSystem			_timeSystem;
		SceneManager		_sceneManager;
		PhysicsSystem2D		_physics;
		GameServerScene*	_gameServerScene{};

		float				_accTime{};
		const float			_fixedTime = 1 / 60.f;
	};
}
