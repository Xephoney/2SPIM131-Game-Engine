#include <engpch.h>


#include "OpenGLVertexArray.h"
#include "Engine/Renderer/Buffer.h"
#include <glad/glad.h>

namespace Engine
{
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::None: return 0;
			case ShaderDataType::Float: return    GL_FLOAT;
			case ShaderDataType::Float2: return   GL_FLOAT;
			case ShaderDataType::Float3: return   GL_FLOAT;
			case ShaderDataType::Float4: return   GL_FLOAT;
			case ShaderDataType::Mat3: return     GL_FLOAT;
			case ShaderDataType::Mat4: return     GL_FLOAT;
			case ShaderDataType::Int: return      GL_INT;
			case ShaderDataType::Int2: return     GL_INT;
			case ShaderDataType::Int3: return     GL_INT;
			case ShaderDataType::Int4: return     GL_INT;
			case ShaderDataType::Bool: return     GL_BOOL;
			default: return 0;
		}
	}
	OpenGLVertexArray::OpenGLVertexArray()
	{
		glCreateVertexArrays(1, &m_RenderID);

	}

	OpenGLVertexArray::~OpenGLVertexArray()
	{
		glDeleteVertexArrays(1, &m_RenderID);
	}

	void OpenGLVertexArray::Bind() const
	{
		glBindVertexArray(m_RenderID);
	}

	void OpenGLVertexArray::Unbind() const
	{
		glBindVertexArray(0);
	}

	void OpenGLVertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& buffer_ptr)
	{
		ENGINE_CORE_ASSERT(buffer_ptr->GetLayout().GetElements().size(), "Vertex buffer has no layout! Remember to set Vertex Buffer layout before calling Add Vertex Buffer");
		glBindVertexArray(m_RenderID);
		buffer_ptr->Bind();
		uint32_t index {0};
		const auto& layout = buffer_ptr->GetLayout();
		for (const auto& element : layout)
		{
			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index,
				element.GetComponentCount(),
				ShaderDataTypeToOpenGLBaseType(element.Type),
				element.Normalized ? GL_TRUE : GL_FALSE,
				layout.GetStride(),
				reinterpret_cast<const void*>(element.Offset));
			index++;
		}

		m_VertexBuffers.push_back(buffer_ptr);
	}

	void OpenGLVertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& iBuffer_ptr)
	{
		glBindVertexArray(m_RenderID);
		iBuffer_ptr->Bind();

		m_IndexBuffer = iBuffer_ptr;
	}
}
