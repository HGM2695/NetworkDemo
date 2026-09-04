#include "GameServerApplication.h"
#include <iostream>

#include "GameProtocol/GamePackets.h"

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
		PacketId packetId = static_cast<PacketId>(packet.header.packetId);
		switch (packetId)
		{

		case PacketId::C2S_JoinRequest:
			std::cout << "C2S_JoinRequest" << std::endl;
			break;

		case PacketId::C2S_ChatRequest:
			std::cout << "C2S_ChatRequest" << std::endl;
			break;

		case PacketId::C2S_MoveRequest:
			std::cout << "C2S_MoveRequest" << std::endl;
			break;

		default:
			std::cout << "유효하지 않은 패킷 Id : " << static_cast<uint16_t>(packetId) << std::endl;
			break;
		}
	}
}
