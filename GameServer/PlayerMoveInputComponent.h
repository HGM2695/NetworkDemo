#pragma once

#include <GMEngine/Component.h>

#include <GameProtocol/GameProtocolTypes.h>

namespace gm
{
	class Rigidbody2DComponent;

	class PlayerMoveInputComponent : public Component
	{
	public:
		void OnInitialize() override;

		void Input(float directionX, bool isJump);

	private:
		Rigidbody2DComponent*	_ownerRigidbody{};

		float					_horizontalMoveSpeed = 300.f;
		float					_jumpImpulse = 800.f;
	};
}
