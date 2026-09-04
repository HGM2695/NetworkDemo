#pragma once

#include "GMEngine/Scene.h"

namespace gm
{
	class TitleScene : public Scene
	{
	protected:
		void OnInitialize() override;
		void OnEnter() override;
		void OnExit() override;

	private:
		void LoadResources();
	};
}
