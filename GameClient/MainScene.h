#pragma once

#include "GMEngine/Scene.h"
#include "GMEngine/WeakGameObjectPtr.h"

#include "GameProtocol/GameProtocolTypes.h"

#include <string>
#include <unordered_map>

namespace gm
{
	class MainScene : public Scene
	{
	public:
		void SpawnPlayer(PlayerId playerId, Vector2 position, const std::wstring& nickName, bool clientPlayer);
		void DestroyPlayer(PlayerId playerId);
		void SetPlayerState(PlayerId playerId, Vector2 position, PlayerMotionState motionState, PlayerFacingDirection facingDirection);

	protected:
		void OnInitialize() override;
		void OnEnter() override;

	private:
		std::unordered_map<PlayerId, WeakGameObjectPtr> _playerList;
	};
}
