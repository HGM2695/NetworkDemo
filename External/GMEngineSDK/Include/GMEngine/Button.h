#pragma once

#include "Border.h"
#include "Event.h"

namespace gm
{
	struct ButtonClickedEvent final : EventType
	{
	};

	class Button final : public Border
	{
	public:
		Button();

		EventPublisher<Button, ButtonClickedEvent> OnClicked;

	protected:
		void OnTick(float deltaTime) override;
		void OnRender(const WidgetGeometry& geometry) override;

	private:
		bool ContainsPoint(const Vector2& point) const;

	private:
		WidgetGeometry	_cachedGeometry{};
		bool			_isPressed = false;
	};
}
