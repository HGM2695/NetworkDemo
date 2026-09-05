#include "GameServerScene.h"

#include "GMEngine/GameObject.h"
#include "GMEngine/Rigidbody2DComponent.h"
#include "GMEngine/BoxCollider2DComponent.h"
#include "GMEngine/TransformComponent.h"

#include "ServerPlayerMovementComponent.h"

namespace gm
{
	void GameServerScene::SpawnPlayer(PlayerId playerId)
	{
		GameObject* player = SpawnGameObject<GameObject>(Vector3{ _defaultPlayerSpawnPosition.x, _defaultPlayerSpawnPosition.y, 0.f });
		player->AddComponent<Rigidbody2DComponent>();
		BoxCollider2DComponent* collider = player->AddComponent<BoxCollider2DComponent>();
		collider->SetSize(Vector2{ 60.f, 100.f });
		player->AddComponent<ServerPlayerMovementComponent>();
		_playerList[playerId] = player->GetWeakPtr();
	}

	void GameServerScene::DestroyPlayer(PlayerId playerId)
	{
		auto iter = _playerList.find(playerId);
		iter->second->Destroy();
		_playerList.erase(iter);
	}

	void GameServerScene::ApplyMoveInput(PlayerId playerId, float directionX, bool isJump)
	{
		auto iter = _playerList.find(playerId);
		if (iter == _playerList.end())
			return;

		if (iter->second.IsValid() == false)
			return;

		ServerPlayerMovementComponent* moveComponent = iter->second->GetComponent<ServerPlayerMovementComponent>();
		moveComponent->ApplyInput(directionX, isJump);
	}

	Vector2 GameServerScene::GetPlayerPosition(PlayerId playerId)
	{
		if (_playerList.find(playerId) == _playerList.end())
			return Vector2{};

		return _playerList[playerId]->GetComponent<TransformComponent>()->GetPosition2D();
	}

	PlayerMotionState GameServerScene::GetPlayerMotionState(PlayerId playerId)
	{
		auto iter = _playerList.find(playerId);
		if (iter == _playerList.end())
			return PlayerMotionState::Idle;

		ServerPlayerMovementComponent* movementComponent = iter->second->GetComponent<ServerPlayerMovementComponent>();
		return movementComponent->GetMotionState();
	}

	PlayerFacingDirection GameServerScene::GetPlayerFacingDirection(PlayerId playerId)
	{
		auto iter = _playerList.find(playerId);
		if (iter == _playerList.end())
			return PlayerFacingDirection::Right;

		ServerPlayerMovementComponent* movementComponent = iter->second->GetComponent<ServerPlayerMovementComponent>();
		return movementComponent->GetFacingDirection();
	}

	void GameServerScene::OnInitialize()
	{
		float MapWidth = 1600.f;
		float MapHeight = 900.f;
		float BoundaryThickness = 100.f;
		const float halfMapWidth = MapWidth * 0.5f;
		const float halfMapHeight = MapHeight * 0.5f;
		const float halfBoundaryThickness = BoundaryThickness * 0.5f;

		GameObject* floorObject = SpawnGameObject<GameObject>(Vector3{ 0.f, -halfMapHeight - halfBoundaryThickness, 0.f });
		BoxCollider2DComponent* floorCollider = floorObject->AddComponent<BoxCollider2DComponent>();
		floorCollider->SetSize(Vector2{ MapWidth + BoundaryThickness * 2.f, BoundaryThickness });

		GameObject* leftWallObject = SpawnGameObject<GameObject>(Vector3{ -halfMapWidth - halfBoundaryThickness, 0.f, 0.f });
		BoxCollider2DComponent* leftWallCollider = leftWallObject->AddComponent<BoxCollider2DComponent>();
		leftWallCollider->SetSize(Vector2{ BoundaryThickness, MapHeight });

		GameObject* rightWallObject = SpawnGameObject<GameObject>(Vector3{ halfMapWidth + halfBoundaryThickness, 0.f, 0.f });
		BoxCollider2DComponent* rightWallCollider = rightWallObject->AddComponent<BoxCollider2DComponent>();
		rightWallCollider->SetSize(Vector2{ BoundaryThickness, MapHeight });
	}
}
