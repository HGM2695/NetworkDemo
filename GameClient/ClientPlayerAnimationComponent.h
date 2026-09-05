#pragma once

#include "GMEngine/Component.h"

#include "GameProtocol/GameProtocolTypes.h"

namespace gm
{
	class AnimatedSpriteComponent;
	class TransformComponent;

	class ClientPlayerAnimationComponent : public Component
	{
	public:
		void SetMotionState(PlayerMotionState motionState);
		void SetFacingDirection(PlayerFacingDirection facingDirection);

	protected:
		void OnInitialize() override;

	private:
		AnimatedSpriteComponent*	_animatedSprite{};
		TransformComponent*			_transform{};
		PlayerMotionState			_motionState = PlayerMotionState::Idle;
	};
}
