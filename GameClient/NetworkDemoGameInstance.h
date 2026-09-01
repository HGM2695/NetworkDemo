#pragma once

#include "GMEngine/GameInstance.h"

namespace gm
{
	class NetworkDemoGameInstance : public GameInstance
	{
	protected:
		virtual bool OnInitialize() override;

	private:
		void SetupScenes();
	};
}
