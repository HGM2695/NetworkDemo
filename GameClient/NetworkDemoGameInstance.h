#pragma once

#include "GMEngine/GameInstance.h"

#include "NetworkCore/WinsockRuntime.h"
#include "NetworkCore/TcpClientService.h"
#include "NetworkCore/PacketTypes.h"

namespace gm
{
	class NetworkDemoGameInstance : public GameInstance
	{
	protected:
		bool	OnInitialize() override;
		void	OnTick(float deltaTime) override;

	private:
		void	SetupScenes();
		void	HandlePacket(PacketView packet);

	private:
		WinsockRuntime		_winsockRuntime{};
		TcpClientService	_clientService{};
	};
}
