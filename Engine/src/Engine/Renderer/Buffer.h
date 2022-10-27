#pragma once

#include <cstdint>
#include <string>
#include <glm/glm.hpp>

#include "Engine/Core.h"
#include "Engine/Log.h"

namespace Engine
{
	enum class ShaderDataType
	{
		None = 0,
		Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};



	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
			case ShaderDataType::Float:     return sizeof(float);
			case ShaderDataType::Float2:    return sizeof(float) * 2;
			case ShaderDataType::Float3:    return sizeof(float) * 3;
			case ShaderDataType::Float4:    return sizeof(float) * 4;
			case ShaderDataType::Mat3:      return sizeof(glm::mat3);
			case ShaderDataType::Mat4:      return sizeof(glm::mat4);
			case ShaderDataType::Int:       return sizeof(int);
			case ShaderDataType::Int2:      return sizeof(int) * 2;
			case ShaderDataType::Int3:      return sizeof(int) * 3;
			case ShaderDataType::Int4:      return sizeof(int) * 4;
			case ShaderDataType::Bool:      return sizeof(bool);
		default: 
			ENGINE_CORE_ASSERT(false,"Invalid or Unknown Shader Data Type |");
			return 0;
		}

	}

	struct BufferElement
	{
		std::string Name;
		uint32_t Offset;
		uint32_t Size;
		ShaderDataType Type;
		bool Normalized {false};
		BufferElement()
		{
			
		}
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Offset(0), Size(ShaderDataTypeSize(type)), Type(type), Normalized(normalized)
		{
			
		}
		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:     return 1;
			case ShaderDataType::Float2:    return 2;
			case ShaderDataType::Float3:    return 3;
			case ShaderDataType::Float4:    return 4;
			case ShaderDataType::Mat3:      return 3 * 3;
			case ShaderDataType::Mat4:      return 4 * 4;
			case ShaderDataType::Int:       return 1;
			case ShaderDataType::Int2:      return 2;
			case ShaderDataType::Int3:      return 3;
			case ShaderDataType::Int4:      return 4;
			case ShaderDataType::Bool:      return 1;
			default:
				ENGINE_CORE_ASSERT(false, "Invalid or Unknown Shader Data Type |");
				return 0;
			}
		}
	};

	class BufferLayout
	{
	public :
		BufferLayout ()
		{
			
		}
		BufferLayout(const std::initializer_list<BufferElement>& element)
			: m_Elements(element)
		{
			CalculateOffsetsAndStride();
			

		}

		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }
		inline uint32_t GetStride() const { return m_Stride; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

	private:
		inline void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;

			for( auto& element : m_Elements)
			{
				element.Offset = offset;
				offset += element.Size;
				m_Stride += element.Size;
			}
		}
		std::vector<BufferElement> m_Elements;
		uint32_t m_Stride;
	};

	class VertexBuffer
	{
	public:
		virtual ~VertexBuffer()
		{
			
		}

		virtual void SetData() {}

		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;
		virtual void SetLayout(const BufferLayout& layout) = 0;
		virtual const BufferLayout& GetLayout() const = 0;
		static VertexBuffer* Create(float* vertices, uint32_t size);

	};

	class IndexBuffer
	{
	public:
		virtual ~IndexBuffer()
		{
			
		}
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetCount() const = 0;

		static IndexBuffer* Create(uint32_t* indices, uint32_t size);

	};
}
