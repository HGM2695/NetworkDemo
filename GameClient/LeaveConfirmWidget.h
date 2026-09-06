#pragma once

#include "GMEngine/Event.h"
#include "GMEngine/UserWidget.h"

namespace gm
{
	struct LeaveConfirmedEvent final : EventType
	{
	};

	class LeaveConfirmWidget : public UserWidget
	{
	public:
		EventPublisher<LeaveConfirmWidget, LeaveConfirmedEvent> OnLeaveConfirmed;

	protected:
		std::unique_ptr<Widget> BuildWidgetTree() override;

	private:
		EventConnection _yesButtonClickConnection{};
		EventConnection _noButtonClickConnection{};
	};
}
