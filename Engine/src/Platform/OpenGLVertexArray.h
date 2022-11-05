#pragma once

#include "Engine/Renderer/VertexArray.h"
#include <vector>

namespace Engine
{
	class OpenGLVertexArray : public VertexArray
	{
	public:
		OpenGLVertexArray();
		~OpenGLVertexArray();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer_ptr) override;
		virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& iBuffer_ptr) override ;
		virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() override { return m_VertexBuffers; };
		virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() override { return m_IndexBuffer; }
		

		//static OpenGLVertexArray* Create();
	private:
		uint32_t m_RenderID;
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
	};

}
