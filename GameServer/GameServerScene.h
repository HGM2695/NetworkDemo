#pragma once

#include "GMEngine/Scene.h"
#include "GMEngine/WeakGameObjectPtr.h"

#include "GameProtocol/GameProtocolTypes.h"

#include <unordered_map>

namespace gm
{
	class GameServerScene : public Scene
	{
	public:
		void		SpawnPlayer(PlayerId playerId);
		void		DestroyPlayer(PlayerId playerId);
		void		ApplyMoveInput(PlayerId playerId, float directionX, bool isJump);

		Vector2		GetPlayerPosition(PlayerId playerId);

		Vector2	GetDefaultPlayerSpawnPosition() { return _defaultPlayerSpawnPosition; }

	protected:
		void	OnInitialize() override;

	private:
		std::unordered_map<PlayerId, WeakGameObjectPtr> _playerList;
		const Vector2									_defaultPlayerSpawnPosition = { 0.f, -200.f };
	};
}
