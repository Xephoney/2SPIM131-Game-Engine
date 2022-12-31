#pragma once
#include <memory>

#include "Scene.h"


namespace Engine
{
	class SceneSerializer
	{
	public:
		SceneSerializer(const std::shared_ptr<Scene>& scene);

		void SerializeText(const std::string& filepath);
		void SerializeBinary(const std::string& filepath);

		bool DeserializeText(const std::string& filepath);
		bool DeserializeBinary(const std::string& filepath);

	private:
		std::shared_ptr<Scene> m_Scene;
	};
}
