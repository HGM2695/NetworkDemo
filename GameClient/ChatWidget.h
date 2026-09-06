#pragma once

#include "GMEngine/UserWidget.h"

namespace gm
{
	class InputTextBox;

	class ChatWidget : public UserWidget
	{
	public:
		void SetInputActive(bool isActive);
		bool IsInputActive() const;

	protected:
		std::unique_ptr<Widget> BuildWidgetTree() override;
		void OnTick(float deltaTime) override;

	private:
		InputTextBox* _chatInput{};
	};
}
