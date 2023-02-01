#pragma once

#include <utility>

namespace Engine
{
	class Input
	{
	public:
		static bool isKeyPressed(int keycode) { return s_instance->isKeyPressedImpl(keycode); }

		static bool isMouseButtonPressed(int button) { return s_instance->isMouseButtonPressedImpl(button); }
		static std::pair<float, float> getMousePosition() { return s_instance->getMousePositionImpl(); }

	protected:
		virtual bool isKeyPressedImpl(int keycode) = 0;
		
		virtual bool isMouseButtonPressedImpl(int button) = 0;
		virtual std::pair<float, float> getMousePositionImpl() = 0;

	private:
		static Input* s_instance;
	};
}