#include "Engine/engpch.h"
#include "LayerStack.h"
namespace Engine
{
	LayerStack::LayerStack()
	{

	}

	LayerStack::~LayerStack()
	{
		for (Layer* l : m_Layers)
			delete l;
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_LayersInsertIndex, layer);
		m_LayersInsertIndex++;
		layer->OnAttach();
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
		overlay->OnAttach();
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		const auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if(it != m_Layers.end())
		{
			m_Layers.erase(it);
			m_LayersInsertIndex--;
		}
	}

	void LayerStack::PopOverlay(Layer* overlay)
	{
		const auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
			m_Layers.erase(it);
	}
}
