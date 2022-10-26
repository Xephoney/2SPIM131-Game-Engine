#pragma once

namespace Engine
{
	enum RenderAPI
	{
		none = 0,
		OpenGL = 1,
		DirectX12 = 2,
		Vulkan = 3
	};

	class Renderer
	{
	public:
		RenderAPI static GetAPI() { return s_api; };

		//TODO : Lag SetAPI() 
	private:
		static RenderAPI s_api;
	};
}
