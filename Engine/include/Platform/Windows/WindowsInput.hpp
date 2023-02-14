#pragma once

#include "include/Input/Input.hpp"

namespace eg
{

	class WindowsInput : public Input
	{
	protected:
		bool isKeyPressedImpl(int keycode) override;

		bool isMouseButtonPressedImpl(int button) override;
		std::pair<float, float> getMousePositionImpl() override;
	};

} // namespace eg