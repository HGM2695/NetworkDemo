#pragma once

#include "GMEngine/UserWidget.h"

#include <string>

namespace gm
{
	class TextBlock;

	class NameTagWidget : public UserWidget
	{
	public:
		explicit NameTagWidget(const std::wstring& nickname);

		void SetNickname(const std::wstring& nickname);

	protected:
		std::unique_ptr<Widget> BuildWidgetTree() override;

	private:
		std::wstring	_nickname{};
		TextBlock*		_nicknameText{};
	};
}
