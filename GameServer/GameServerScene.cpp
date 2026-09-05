#include "GameServerScene.h"

#include "GMEngine/GameObject.h"
#include "GMEngine/BoxCollider2DComponent.h"

namespace gm
{
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