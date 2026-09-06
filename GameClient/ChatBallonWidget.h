#pragma once

#include "GMEngine/UserWidget.h"

#include <cstddef>
#include <string>

namespace gm
{
	class Border;
	class TextBlock;

	class ChatBallonWidget : public UserWidget
	{
	public:
		void					SetMessage(const std::wstring& message);

	protected:
		std::unique_ptr<Widget> BuildWidgetTree() override;
		void					OnTick(float deltaTime) override;

	private:
		std::wstring	WrapMessage(const std::wstring& message) const;
		void			UpdateLayout();

	private:
		float			_accTime{};
		float			_visibleDuration = 1.5f;

		std::wstring	_message{};
		Widget*			_root{};
		Border*			_body{};
		TextBlock*		_messageText{};
	};
}
