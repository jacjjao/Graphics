#include "include/Core/LayerStack.hpp"

namespace Engine
{
	void LayerStack::pushLayer(Layer* layer)
	{
		m_layers.emplace(m_layers.cbegin() + m_layerInsertIdx, layer);
		m_layerInsertIdx++;
	}

	void LayerStack::pushOverlay(Layer* overlay)
	{
		m_layers.emplace_back(overlay);
	}

	void LayerStack::popLayer(Layer* layer)
	{
		const auto it = std::ranges::find_if(m_layers.begin(), m_layers.end(), [&layer](const auto& ptr) {
			return ptr.get() == layer;
		});
		if (it != m_layers.end())
		{
			it->release();
			m_layers.erase(it);
			m_layerInsertIdx--;
		}
	}

	void LayerStack::popOverlay(Layer* overlay)
	{
		const auto it = std::ranges::find_if(m_layers.begin(), m_layers.end(), [&overlay](const auto& ptr) {
			return ptr.get() == overlay;
		});
		if (it != m_layers.end())
		{
			it->release();
			m_layers.erase(it);
		}
	}


}