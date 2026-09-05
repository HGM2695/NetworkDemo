#include "ServerPlayerMovementComponent.h"

#include "GMEngine/GameObject.h"
#include "GMEngine/Rigidbody2DComponent.h"

#include <iostream>

namespace gm
{
	void ServerPlayerMovementComponent::OnInitialize()
	{
		_ownerRigidbody = GetOwner().GetComponent<Rigidbody2DComponent>();
		if (_ownerRigidbody == nullptr)
			std::cout << "생성된 서버 플레이어가 Rigidbody2DComponent를 소유하지 않습니다." << std::endl;
	}

	void ServerPlayerMovementComponent::OnTick(float)
	{
		if (_ownerRigidbody == nullptr)
			return;

		Vector2 velocity = _ownerRigidbody->GetVelocity();
		velocity.x = _moveDirectionX * _horizontalMoveSpeed;
		_ownerRigidbody->SetVelocity(velocity);

		if (_isJumpRequested && _ownerRigidbody->IsGrounded())
			_ownerRigidbody->AddImpulse(Vector2{ 0.f, _jumpImpulse });

		_isJumpRequested = false;
	}

	void ServerPlayerMovementComponent::ApplyInput(float directionX, bool isJump)
	{
		_moveDirectionX = directionX;
		if (directionX < 0.f)
			_facingDirection = PlayerFacingDirection::Left;
		else if (directionX > 0.f)
			_facingDirection = PlayerFacingDirection::Right;

		if (isJump)
			_isJumpRequested = true;
	}

	PlayerMotionState ServerPlayerMovementComponent::GetMotionState() const
	{
		if (_ownerRigidbody == nullptr)
			return PlayerMotionState::Idle;

		const Vector2 velocity = _ownerRigidbody->GetVelocity();
		if (_ownerRigidbody->IsGrounded())
			return std::abs(velocity.x) > 0.01f ? PlayerMotionState::Walk : PlayerMotionState::Idle;

		return velocity.y > 0.f ? PlayerMotionState::Jump : PlayerMotionState::Fall;
	}
}
