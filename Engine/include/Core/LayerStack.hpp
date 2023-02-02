#pragma once

#include "include/Core/Core.hpp"
#include "include/Core/Layer.hpp"

#include <vector>
#include <memory>

namespace Engine
{
	class LayerStack
	{
	private:
		using ContainerType = std::vector<std::unique_ptr<Layer>>;

	public:
		void pushLayer(Layer* layer);
		void pushOverlay(Layer* overlay);

		void popLayer(Layer* layer);
		void popOverlay(Layer* overlay);

		ContainerType::iterator begin() noexcept { return m_layers.begin(); }
		ContainerType::iterator end() noexcept { return m_layers.end(); }

	private:
		ContainerType m_layers;
		int m_layerInsertIdx = 0;
	};
}


