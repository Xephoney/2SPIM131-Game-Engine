#pragma once

#include "Engine/Layer.h"

#include "Engine/Events/ApplicationEvent.h"


namespace Engine
{
	class ENGINE_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnImGuiRender() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
	};
}
