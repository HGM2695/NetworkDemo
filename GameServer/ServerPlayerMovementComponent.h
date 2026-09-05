#pragma once

#include <GMEngine/Component.h>

#include <GameProtocol/GameProtocolTypes.h>

namespace gm
{
	class Rigidbody2DComponent;

	class ServerPlayerMovementComponent : public Component
	{
	public:
		void					ApplyInput(float directionX, bool isJump);
		PlayerMotionState		GetMotionState() const;
		PlayerFacingDirection	GetFacingDirection() const { return _facingDirection; }

	protected:
		void	OnInitialize() override;
		void	OnTick(float deltaTime) override;

	private:
		Rigidbody2DComponent*	_ownerRigidbody{};

		float					_moveDirectionX{};
		bool					_isJumpRequested{};
		PlayerFacingDirection	_facingDirection = PlayerFacingDirection::Right;

		float					_horizontalMoveSpeed = 300.f;
		float					_jumpImpulse = 800.f;
	};
}
