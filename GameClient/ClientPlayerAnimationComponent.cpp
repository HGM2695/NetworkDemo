#include "ClientPlayerAnimationComponent.h"

#include "GMEngine/AnimatedSpriteComponent.h"
#include "GMEngine/GameObject.h"
#include "GMEngine/SpriteAnimator.h"
#include "GMEngine/TransformComponent.h"

#include <cmath>

namespace gm
{
	void ClientPlayerAnimationComponent::OnInitialize()
	{
		_animatedSprite = GetOwner().GetComponent<AnimatedSpriteComponent>();
		_transform = GetOwner().GetComponent<TransformComponent>();
		GM_ASSERT(_animatedSprite, "ClientPlayerAnimationComponent 소유자의 AnimatedSpriteComponent가 존재하지 않습니다.");
		GM_ASSERT(_transform, "ClientPlayerAnimationComponent 소유자의 TransformComponent가 존재하지 않습니다.");
	}

	void ClientPlayerAnimationComponent::SetMotionState(PlayerMotionState motionState)
	{
		if (_animatedSprite == nullptr || _motionState == motionState)
			return;

		_motionState = motionState;
		SpriteAnimator& animator = _animatedSprite->GetAnimator();

		switch (_motionState)
		{
		case PlayerMotionState::Idle:
			GM_ASSERT_RETURN(animator.Play(L"Idle"), "플레이어 Idle 애니메이션 재생 실패");
			break;
		case PlayerMotionState::Walk:
			GM_ASSERT_RETURN(animator.Play(L"Walk"), "플레이어 Walk 애니메이션 재생 실패");
			break;
		case PlayerMotionState::Jump:
		case PlayerMotionState::Fall:
			GM_ASSERT_RETURN(animator.Play(L"Jump"), "플레이어 Jump 애니메이션 재생 실패");
			break;
		}
	}

	void ClientPlayerAnimationComponent::SetFacingDirection(PlayerFacingDirection facingDirection)
	{
		if (_transform == nullptr)
			return;

		Vector3 scale = _transform->GetScale();
		scale.x = facingDirection == PlayerFacingDirection::Left ? std::abs(scale.x) : -std::abs(scale.x);
		_transform->SetScale(scale);
	}
}
