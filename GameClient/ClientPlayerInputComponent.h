#pragma once

#include "GMEngine/Component.h"

namespace gm
{
	class NetworkDemoGameInstance;

	class ClientPlayerInputComponent : public Component
	{
	public:
		void	OnInitialize() override;
		void	OnTick(float deltaTime) override;

	private:
		NetworkDemoGameInstance*	_gameInstance{};
		float						_prevDirection = 0.f;
	};
}
