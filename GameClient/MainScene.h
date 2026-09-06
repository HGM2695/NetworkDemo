#pragma once

#include "GMEngine/Scene.h"
#include "GMEngine/Event.h"
#include "GMEngine/WeakGameObjectPtr.h"

#include "GameProtocol/GameProtocolTypes.h"

#include <string>
#include <unordered_map>

namespace gm
{
	class LeaveConfirmWidget;

	class MainScene : public Scene
	{
	public:
		void SpawnPlayer(PlayerId playerId, Vector2 position, const std::wstring& nickName, bool clientPlayer);
		void DestroyPlayer(PlayerId playerId);
		void ClearPlayers();
		void SetPlayerState(PlayerId playerId, Vector2 position, PlayerMotionState motionState, PlayerFacingDirection facingDirection);
		void ShowLeaveConfirmation();

	protected:
		void OnInitialize() override;
		void OnTick(float deltaTime) override;
		void OnEnter() override;
		void OnExit() override;

	private:
		std::unordered_map<PlayerId, WeakGameObjectPtr> _playerList;

		LeaveConfirmWidget* _leaveConfirmWidget{};
		EventConnection		_leaveConfirmedConnection{};
	};
}
