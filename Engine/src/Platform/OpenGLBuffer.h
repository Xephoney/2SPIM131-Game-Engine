#pragma once
#include "Engine/Renderer/Buffer.h"


namespace Engine
{
	class OpenGLVertexBuffer : public VertexBuffer
	{
	public:
		OpenGLVertexBuffer(float* vertices, uint32_t size);
		~OpenGLVertexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetLayout(const BufferLayout& layout) override ;
		const BufferLayout& GetLayout() const override;

	private:
		uint32_t m_RendererID{};
		BufferLayout m_Layout;
	};

	class OpenGLIndexBuffer : public IndexBuffer
	{
	public:
		OpenGLIndexBuffer(uint32_t* indices, uint32_t count);
		~OpenGLIndexBuffer() override;

		virtual void Bind() const override;
		virtual void Unbind() const override;

		[[nodiscard]] uint32_t GetCount() const override { return m_count; }
	private:
		uint32_t m_count;
		uint32_t m_RendererID{};
	};
}
