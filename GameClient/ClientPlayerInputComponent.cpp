#include "ClientPlayerInputComponent.h"

#include "GMEngine/Application.h"
#include "GMEngine/Input.h"

#include "NetworkDemoGameInstance.h"

namespace gm
{
	void ClientPlayerInputComponent::OnInitialize()
	{
		_gameInstance = static_cast<NetworkDemoGameInstance*>(&APPLICATION.GetGameInstance());
	}

	void ClientPlayerInputComponent::OnTick(float deltaTime)
	{
		bool needSendPacket = false;

		float directionX = APPLICATION.GetInput().GetMoveAxisX();
		if (directionX != _prevDirection)
			needSendPacket = true;
		_prevDirection = directionX;

		bool isJump = APPLICATION.GetInput().IsKeyDown(KeyCode::Up);
		if (isJump)
			needSendPacket = true;

		if (needSendPacket)
			_gameInstance->RequestMove(directionX, isJump);
	}
}
