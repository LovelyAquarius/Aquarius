#pragma once
#include "core/AquariusCore.h"
#include "AQScene.h"
namespace Aquarius
{
	class AQUARIUS_API SceneSerializer
	{
	public:
		SceneSerializer(const AQRef<AQScene>& scene);

		void Serialize(const std::string& filepath);
		void SerializeAtRuntime(const std::string& filepath);
		AQBOOL Deserialize(const std::string& filepath);
		AQBOOL DeserializeAtRuntime(const std::string& filepath);
	private:
		AQRef<AQScene> m_Scene;
	};


}


