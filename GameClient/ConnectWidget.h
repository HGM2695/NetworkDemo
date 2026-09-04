#pragma once

#include "GMEngine/UserWidget.h"
#include "GMEngine/Event.h"

namespace gm
{
	class ConnectWidget : public UserWidget
	{
	protected:
		virtual std::unique_ptr<Widget>	BuildWidgetTree() override;

	private:
		EventConnection _buttonClickConnection;
	};
}
