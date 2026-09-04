#include "GameServerApplication.h"
#include <iostream>


namespace gm
{
	bool GameServerApplication::Initialize()
	{
		if (_winsockRuntime.Initialize() == false)
		{
			std::cout << "winSock 초기화 실패" << std::endl;
			return 0;
		}

		if (_serverService.Initialize(_port,
			[this](TcpSession::SessionId id, PacketView packet)
			{
				HandlePacket(id, packet);
			}) == false)
		{
			std::cout << "Server Service 초기화 실패" << std::endl;
			return 0;
		}

		return true;
	}

	void GameServerApplication::Run()
	{
		while (true)
		{
			_timeSystem.Tick();
			const float deltaTime = _timeSystem.GetDeltaTime();

			_serverService.Tick();
		}
	}

	void GameServerApplication::HandlePacket(TcpSession::SessionId id, PacketView packet)
	{

	}
}
