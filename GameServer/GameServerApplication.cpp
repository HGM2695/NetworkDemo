#include "GameServerApplication.h"
#include "GameServerScene.h"

#include <iostream>

#include "GMEngine/StringUtil.h"

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

		_gameServerScene = static_cast<GameServerScene*>(_sceneManager.CreateScene<GameServerScene>(L"GameServerScene"));
		if (_gameServerScene == nullptr)
			return false;
		_sceneManager.RequestSceneChange(L"GameServerScene");

		return true;
	}

	void GameServerApplication::Run()
	{
		while (true)
		{
			_timeSystem.Tick();
			const float deltaTime = _timeSystem.GetDeltaTime();

			_accTime += deltaTime;
			while (_accTime >= _fixedTime)
			{
				_accTime -= _fixedTime;

				_sceneManager.BeginFrame();
				_sceneManager.Tick(TickGroup::GameLogic, _fixedTime);
				_sceneManager.Tick(TickGroup::Movement, _fixedTime);
				_physics.Simulate(*_gameServerScene, _fixedTime);
				_sceneManager.EndFrame();

				_serverService.Tick();
			}
		}
	}

	void GameServerApplication::HandlePacket(TcpSession::SessionId sessionId, PacketView packet)
	{
		PacketId packetId = static_cast<PacketId>(packet.header.packetId);
		std::span<const std::byte> payload = packet.payload;

		switch (packetId)
		{
		case PacketId::C2S_JoinRequest:
		{
			if (payload.empty() || payload.size() > MaxNicknameByteLength)
				return;

			if (_playerIdList.find(sessionId) != _playerIdList.end())
				return;

			S2CJoinAccepted accepetedPacket{};
			accepetedPacket.playerId = _nextPlayerId;

			const std::span<const std::byte> view = std::as_bytes(std::span{ &accepetedPacket, 1 });
			if (_serverService.Send(sessionId, static_cast<uint16_t>(PacketId::S2C_JoinAccepted), view) == false)
				return;

			std::string utf8NickName;
			utf8NickName.resize(payload.size());
			memcpy(utf8NickName.data(), payload.data(), payload.size());

			SendAllPlayerList(sessionId);

			_gameServerScene->SpawnPlayer(_nextPlayerId);
			_playerIdList[sessionId] = PlayerInfo{ _nextPlayerId, Utf8ToWide(utf8NickName.data()) };
			++_nextPlayerId;

			BroadCastPlayerJoin(_nextPlayerId - 1, _gameServerScene->GetDefaultPlayerSpawnPosition(), payload);
		}
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

	void GameServerApplication::SendAllPlayerList(TcpSession::SessionId target)
	{
		for (const auto& [sessionId, playerInfo] : _playerIdList)
		{
			S2CPlayerJoinedPrefix prefix{};
			prefix.playerId = playerInfo.id;
			Vector2 position = _gameServerScene->GetPlayerPosition(playerInfo.id);
			prefix.positionX = position.x;
			prefix.positionY = position.y;

			std::string utf8NickName = WideToUtf8(playerInfo.nickName.data());
			std::vector<std::byte> payload(sizeof(prefix) + utf8NickName.size());
			memcpy(payload.data(), &prefix, sizeof(prefix));
			memcpy(payload.data() + sizeof(prefix), utf8NickName.data(), utf8NickName.size());
			_serverService.Send(target, static_cast<std::uint16_t>(PacketId::S2C_PlayerJoined), std::span<const std::byte>{payload});
		}
	}

	void GameServerApplication::BroadCastPlayerJoin(PlayerId playerId, Vector2 position, std::span<const std::byte> nickName)
	{
		S2CPlayerJoinedPrefix prefix{};
		prefix.playerId = playerId;
		prefix.positionX = position.x;
		prefix.positionY = position.y;

		std::vector<std::byte> payload(sizeof(prefix) + nickName.size());
		memcpy(payload.data(), &prefix, sizeof(prefix));
		memcpy(payload.data() + sizeof(prefix), nickName.data(), nickName.size());

		for (const auto& [sessionId, playerInfo] : _playerIdList)
		{
			_serverService.Send(sessionId, static_cast<std::uint16_t>(PacketId::S2C_PlayerJoined), std::span<const std::byte>{payload});
		}
	}
}
