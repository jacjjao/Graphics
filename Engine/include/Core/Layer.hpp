#pragma once

#include "include/Core/Core.hpp"
#include "include/Events/Event.hpp"

namespace Engine
{
	class Layer
	{
	public:
		virtual ~Layer() = default;

		virtual void onAttach() = 0;
		virtual void onDetach() = 0;
		virtual void onUpdate() = 0;
		virtual void onEvent(Event& event) = 0;
	};
}
