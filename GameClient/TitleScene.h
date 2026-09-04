#pragma once

#include "GMEngine/Scene.h"
#include "GMEngine/Event.h"

namespace gm
{
	class ConnectWidget;

	class TitleScene : public Scene
	{
	protected:
		void	OnInitialize() override;
		void	OnEnter() override;
		void	OnExit() override;

	private:
		void	OnConnectButtonClicked();

	private:
		EventConnection			_buttonClickConnection{};
		ConnectWidget*			_connectWidget{};
	};
}
