#pragma once

#include "GMEngine/UserWidget.h"

namespace gm
{
	class ConnectWidget : public UserWidget
	{
		virtual std::unique_ptr<Widget>	BuildWidgetTree() override;
	};
}
