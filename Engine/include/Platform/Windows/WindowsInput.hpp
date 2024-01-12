#pragma once

#include "include/Core/Core.hpp"
#include "include/Input/Input.hpp"

namespace eg
{

	class EG_API WindowsInput : public Input
	{
	protected:
		bool isKeyPressedImpl(int keycode) override;

		bool isMouseButtonPressedImpl(int button) override;
		std::pair<float, float> getMousePositionImpl() override;
	};

} // namespace eg