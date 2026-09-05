#include "MainScene.h"
#include "NameTagWidget.h"

#include "GMEngine/GameObject.h"
#include "GMEngine/CameraComponent.h"
#include "GMEngine/CameraManager.h"
#include "GMEngine/Application.h"
#include "GMEngine/PathUtil.h"
#include "GMEngine/AnimatedSpriteComponent.h"
#include "GMEngine/TransformComponent.h"
#include "GMEngine/SpriteAnimator.h"
#include "GMEngine/WidgetComponent.h"

namespace gm
{
	void MainScene::SpawnPlayer(PlayerId playerId, Vector2 position, const std::wstring& nickName)
	{
		GameObject* player = SpawnGameObject<GameObject>(Vector3{ position.x, position.y, 0.f});
		player->GetComponent<TransformComponent>()->SetScale(Vector3{ 450.f, 450.f, 1.f });
		AnimatedSpriteComponent* spriteComponent = player->AddComponent<AnimatedSpriteComponent>();
		SpriteAnimator& animator = spriteComponent->GetAnimator();
		GM_ASSERT_RETURN(animator.AddClip(L"Idle", L"Player.Idle"), " 플레이어 Idle 클립 추가 실패");
		GM_ASSERT_RETURN(animator.AddClip(L"Jump", L"Player.Jump"), " 플레이어 Jump 클립 추가 실패");
		GM_ASSERT_RETURN(animator.AddClip(L"Walk", L"Player.Walk"), " 플레이어 Walk 클립 추가 실패");
		GM_ASSERT_RETURN(animator.Play(L"Idle"), "플레이어 Idle 애니메이션 재생 실패");

		WidgetComponent* nameTagComponent = player->AddComponent<WidgetComponent>();
		nameTagComponent->SetUserWidget<NameTagWidget>(nickName);
		nameTagComponent->SetScreenOffset(Vector2{ 0.f, 100.f });

		_playerList[playerId] = player->GetWeakPtr();
	}

	void MainScene::DestroyPlayer(PlayerId playerId)
	{
		auto Iter = _playerList.find(playerId);
		if (Iter == _playerList.end())
			return;

		Iter->second->Destroy();
		_playerList.erase(Iter);
	}

	void MainScene::SetPlayerPosition(PlayerId playerId, Vector2 position)
	{
		auto Iter = _playerList.find(playerId);
		if (Iter == _playerList.end())
			return;

		Iter->second->GetComponent<TransformComponent>()->SetPosition2D(position);
	}

	void MainScene::OnInitialize()
	{
		GameObject* cameraObject = SpawnGameObject<GameObject>();
		CameraComponent* camera = cameraObject->AddComponent<CameraComponent>();

		const float width = static_cast<float>(APPLICATION.GetWidth());
		const float height = static_cast<float>(APPLICATION.GetHeight());

		camera->SetOrthographic(width, height);
		GetCameraManager()->RegisterCamera(L"MainCamera", camera);
	}

	void MainScene::OnEnter()
	{
		GetCameraManager()->SetActiveCamera(L"MainCamera");
	}
}
