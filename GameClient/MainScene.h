#pragma once

#include "GMEngine/Scene.h"

namespace gm
{
	class MainScene : public Scene
	{
	protected:
		void OnInitialize() override;
		void OnEnter() override;
	};
}
