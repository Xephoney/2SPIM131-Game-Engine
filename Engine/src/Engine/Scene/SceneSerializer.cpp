
#include "engpch.h"

/*
#include "SceneSerializer.h"
#include "Entity.h"
#include "Components.h"
#include <fstream>

#include "yaml-cpp/yaml.h"
#define key YAML::Key
#define val YAML::Value
#define begin YAML::BeginMap;
#define end YAML::EndMap;

namespace Engine
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
		return out;
	}

	YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
		return out;
	}


	SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene)
		: m_Scene(scene)
	{
		
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity)
	{
		out << YAML::BeginMap;
		out << key << "Entity" << val << (uint32_t)entity.m_EntityHandle; //TODO : UUID instead of entityHandle
		
		if(entity.HasComponent<Tag>())
		{
			out << key << "TagComponent";
			out << begin;
			auto & tag = entity.GetComponent<Tag>().tag;
			out << key << "Tag" << val << tag;
			out << end;
		}
		if(entity.HasComponent<Transform>())
		{
			out << key << "TransformComponent";
			out << begin;
			auto & transform = entity.GetComponent<Transform>();
			out << key << "Position" << val << transform.position;
			out << key << "Rotation" << val << transform.euler_rotation;
			out << key << "Scale" << val << transform.scale;
			out << end;
		}

		if(entity.HasComponent<Rigidbody>())
		{
			//TODO : ADD RIGIDBODY PARAMETERS FOR SERIALIZATION
		}

		if (entity.HasComponent<AudioSource>())
		{
			//TODO : ADD SOUNDFILE LOCATION FOR SERIALIZATION
		}
		out << YAML::EndMap;
	}

	void SceneSerializer::SerializeText(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap; 
		out << YAML::Key << "Scene";
		out << YAML::Value << "Name";
		out << YAML::Key << "Entities";
		out << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityID)
		{
			Entity entity = { entityID, m_Scene.get() };
			if (!entity)
				return;

			SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;

		std::ofstream fOut(filepath);
		fOut << out.c_str();
	}

	void SceneSerializer::SerializeBinary(const std::string& filepath)
	{
		ENGINE_CORE_ASSERT(false, "NOT IMPLEMENTED");
	}
	
	bool SceneSerializer::DeserializeText(const std::string& filepath)
	{
		ENGINE_CORE_ASSERT(false, "NOT IMPLEMENTED");
		return false;
	}

	bool SceneSerializer::DeserializeBinary(const std::string& filepath)
	{
		ENGINE_CORE_ASSERT(false, "NOT IMPLEMENTED");
		return false;
	}
}
*/
