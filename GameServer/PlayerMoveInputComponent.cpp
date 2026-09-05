#include "PlayerMoveInputComponent.h"

#include "GMEngine/GameObject.h"
#include "GMEngine/Rigidbody2DComponent.h"

#include <iostream>

namespace gm
{
	void PlayerMoveInputComponent::OnInitialize()
	{
		_ownerRigidbody = GetOwner().GetComponent<Rigidbody2DComponent>();
		if (_ownerRigidbody == nullptr)
			std::cout << "생성된 서버 플레이어가 Rigidbody2DComponent를 소유하지 않습니다." << std::endl;
	}

	void PlayerMoveInputComponent::Input(float directionX, bool isJump)
	{
		if (_ownerRigidbody == nullptr)
			return;

		const float verticalVelocity = _ownerRigidbody->GetVelocity().y;
		_ownerRigidbody->SetVelocity(Vector2{ directionX * _horizontalMoveSpeed, verticalVelocity });

		if (isJump && _ownerRigidbody->IsGrounded())
			_ownerRigidbody->AddImpulse(Vector2{ 0.f, _jumpImpulse });
	}
}
