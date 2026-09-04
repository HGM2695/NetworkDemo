#pragma once

#include "GMEngine/GameInstance.h"

#include "NetworkCore/WinsockRuntime.h"
#include "NetworkCore/TcpClientService.h"
#include "NetworkCore/PacketTypes.h"

#include <string>

namespace gm
{
	class NetworkDemoGameInstance : public GameInstance
	{
	public:
		bool	RequestJoin(const std::wstring& address, std::uint16_t portNumber, const std::wstring& nickname);

	protected:
		bool	OnInitialize() override;
		void	OnTick(float deltaTime) override;

	private:
		void	SetupScenes();
		void	HandlePacket(PacketView packet);

		bool	SendJoinPacket();

	private:
		enum class State
		{
			Idle,
			Connecting,
			WaitingForJoinAccepted,
			Joined
		};

	private:
		WinsockRuntime		_winsockRuntime{};
		TcpClientService	_clientService{};

		State				_state{};
		std::string			_clientNickname{};
	};
}
