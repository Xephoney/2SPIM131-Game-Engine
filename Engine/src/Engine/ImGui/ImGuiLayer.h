#pragma once

#include "Engine/Layer.h"

#include "Engine/Events/ApplicationEvent.h"


namespace Engine
{
	class  ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnImGuiRender() override;
		virtual void OnDetach() override;

		void Begin();
		void End();
		void OnUpdate(const double& dt) override;
		void OnEvent(Event& event) override;

		void SetSkipEvent(bool b) { m_SkipEvents = b; }
		void SetBlockEvents(bool b) { m_BlockImGuiEvents = b; }
	private:
		bool m_BlockImGuiEvents = true;
		bool m_SkipEvents = false;
		float m_Time = 0.f;
	};
}
