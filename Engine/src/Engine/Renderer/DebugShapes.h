#pragma once
#include "VertexArray.h"

namespace Engine
{
	enum class DebugShape
	{
		WireBox,
		WireSphere
	};

	class DebugShapes
	{
	public:
		DebugShapes();
		auto GetShape(DebugShape) const -> VertexArray*;
	private:
		std::unordered_map<DebugShape, VertexArray*> map;
	};

	inline DebugShapes::DebugShapes()
	{
		/*std::shared_ptr<VertexArray> va;
		va.reset(VertexArray::Create());
		std::vector<Vertex> verts;
		std::vector<unsigned int> indices;
		float size = 0.5001;
		/*verts.emplace_back(Vertex({ size,-size,size }, { 0,0,0 }, color, { 0,0 }));
		verts.emplace_back(Vertex({ -size,-size,size }, { 0,0,0 }, color, { 0,0 }));
		verts.emplace_back(Vertex({ -size, size,size }, { 0,0,0 }, color, { 0,0 }));
		verts.emplace_back(Vertex({ size, size,size }, { 0,0,0 }, color, { 0,0 }));
		verts.emplace_back(Vertex({ size,-size,-size }, { 0,0,0 }, color, { 0,0 }));
		verts.emplace_back(Vertex({ -size,-size,-size }, { 0,0,0 }, color, { 0,0 }));
		verts.emplace_back(Vertex({ -size, size,-size }, { 0,0,0 }, color, { 0,0 }));
		verts.emplace_back(Vertex({ size, size,-size }, { 0,0,0 }, color, { 0,0 }));
		#1#

		indices.push_back(0);
		indices.push_back(1);

		indices.push_back(0);
		indices.push_back(3);

		indices.push_back(0);
		indices.push_back(4);

		indices.push_back(1);
		indices.push_back(2);

		indices.push_back(1);
		indices.push_back(5);

		indices.push_back(5);
		indices.push_back(6);

		indices.push_back(5);
		indices.push_back(4);

		indices.push_back(4);
		indices.push_back(7);

		indices.push_back(7);
		indices.push_back(3);

		indices.push_back(3);
		indices.push_back(2);

		indices.push_back(2);
		indices.push_back(6);

		indices.push_back(6);
		indices.push_back(7);
		std::shared_ptr<VertexBuffer> vb;
		vb.reset(VertexBuffer::Create(verts));
		vb->SetLayout({
			{ShaderDataType::Float3, "a_Position"},
			{ShaderDataType::Float3, "a_Normal"},
			{ShaderDataType::Float4, "a_Color"},
			{ShaderDataType::Float2, "a_UV"}
			});
		va->AddVertexBuffer(vb);

		std::shared_ptr<IndexBuffer> ib;
		ib.reset(IndexBuffer::Create(indices.data(), indices.size()));
		va->SetIndexBuffer(ib);*/
	}

	inline auto DebugShapes::GetShape(DebugShape shape) const -> VertexArray*
	{
		auto it = map.find(shape);
		if(it != map.end())
		{
			return it->second;
		}
		ENGINE_CORE_ASSERT(false, "SHAPE NOT INITIALIZED, CREATE SHADE VERTEX ARRAY BEFORE USING IT");
		return nullptr;
	}
}
