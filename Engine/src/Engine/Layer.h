#pragma once

#include "Engine/Core.h"
#include "Engine/Events/Event.h"

namespace Engine
{
	class  Layer
	{
	public:
		Layer(const std::string& debugName = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(const double& dt) {}
		virtual void OnImGuiRender() {}
		virtual void OnEvent(Event& event) {}

		const std::string& GetName() const { return m_DebugName; }

	private:
		std::string m_DebugName;
	};
}
