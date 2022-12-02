#include "AQPCH.h"
#include "SceneSerializer.h"
#include "AQEntity.h"

#include "ElementSystem/AQComponents/AQComponentHeader.h"

#include<fstream>
#include<yaml-cpp/yaml.h>
namespace Aquarius
{
	static void SerializeEntity(YAML::Emitter& out, AQEntity entity);
	static void DeserializeComponentsToEntity( AQEntity& entity,YAML::detail::iterator_value& entitydata);
	YAML::Emitter& operator<<(YAML::Emitter& out, const Eigen::Vector3f& value);
	YAML::Emitter& operator<<(YAML::Emitter& out, const Eigen::Vector4f& value);
}
namespace YAML
{
	template<>
	struct convert<Eigen::Vector3f>
	{
		static Node encode(const Eigen::Vector3f& rhs)
		{
			Node node;
			node.push_back(rhs.x());
			node.push_back(rhs.y());
			node.push_back(rhs.z());

			return node;
		}

		static bool decode(const Node& node, Eigen::Vector3f& rhs)
		{
			if (!node.IsSequence() || node.size() != 3)
				return false;
			rhs.x() = node[0].as<float>();
			rhs.y() = node[1].as<float>();
			rhs.z() = node[2].as<float>();
			return true;
		}
	};

	template<>
	struct convert<Eigen::Vector4f>
	{
		static Node encode(const Eigen::Vector4f& rhs)
		{
			Node node;
			node.push_back(rhs.x());
			node.push_back(rhs.y());
			node.push_back(rhs.z());
			node.push_back(rhs.w());
			return node;
		}

		static bool decode(const Node& node, Eigen::Vector4f& rhs)
		{
			if (!node.IsSequence() || node.size() != 4)
				return false;
			rhs.x() = node[0].as<float>();
			rhs.y() = node[1].as<float>();
			rhs.z() = node[2].as<float>();
			rhs.w() = node[3].as<float>();
			return true;
		}
	};
}


namespace Aquarius
{




	SceneSerializer::SceneSerializer(const AQRef<AQScene>& scene):m_Scene(scene){}
	void SceneSerializer::Serialize(const std::string& filepath)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Scene"<<YAML::Value << "Unnamed Scene";
		out << YAML::Key << "Entities" << YAML::Value << YAML::BeginSeq;
		m_Scene->m_Registry.each([&](auto entityid)
		{
				AQEntity entity = { entityid ,m_Scene };
				if (!entity)
					return;
				SerializeEntity(out, entity);
		});
		out << YAML::EndSeq;
		out << YAML::EndMap;
		std::ofstream fout(filepath);
		fout << out.c_str();

	}

	AQBOOL SceneSerializer::Deserialize(const std::string& filepath)
	{
		std::ifstream stream(filepath);
		if (stream)
		{
			std::stringstream strstream;
			strstream << stream.rdbuf();

			YAML::Node data = YAML::Load(strstream.str());
			if (!data["Scene"])
				return false;

			std::string scenename = data["Scene"].as<std::string>();
			AQ_CORE_TRACE("Deserializing scene '{0}' ........", scenename);
			auto entities = data["Entities"];
			if (entities)
			{
				for (auto entity : entities)
				{
					uint64_t uid = entity["Entity"].as<uint64_t>();

					std::string name;
					auto tagcomponent = entity["TagComponent"];
					if (tagcomponent)
						name = tagcomponent["Tag"].as<std::string>();
					AQ_CORE_TRACE("Deserrialized entity with ID = {0}, name = {1}", uid, name);
					AQEntity deserializedentity = m_Scene->CreateEntity(name);
					DeserializeComponentsToEntity(deserializedentity, entity);
					
				}
			}
		}
		else
		{
			AQ_CORE_ERROR("SceneSerializer::Deserialize:Failed to open the file: {0}!", filepath);
			return false;
		}
		return true;
	}


}
namespace Aquarius
{
	YAML::Emitter& operator<<(YAML::Emitter& out, const Eigen::Vector3f& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x() << value.y() << value.z() << YAML::EndSeq;
		return out;
	}
	YAML::Emitter& operator<<(YAML::Emitter& out, const Eigen::Vector4f& value)
	{
		out << YAML::Flow;
		out << YAML::BeginSeq << value.x() << value.y() << value.z() << value.w() << YAML::EndSeq;
		return out;
	}


	void SerializeEntity(YAML::Emitter& out, AQEntity entity)
	{
		out << YAML::BeginMap;
		out << YAML::Key << "Entity" << YAML::Value << "1234567";


		if (entity.HasComponent<AQTagComponent>())
		{
			out << YAML::Key << "TagComponent";
			out << YAML::BeginMap;
			auto& tag = entity.GetComponent<AQTagComponent>()->Tag;
			out << YAML::Key << "Tag" << YAML::Value << tag;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<AQTransformComponent>())
		{
			out << YAML::Key << "TransformComponent";
			out << YAML::BeginMap;
			auto component = entity.GetComponent<AQTransformComponent>();
			out << YAML::Key << "Translation" << YAML::Value << component->Translation;
			out << YAML::Key << "Rotation" << YAML::Value << component->Rotation;
			out << YAML::Key << "Scale" << YAML::Value << component->Scale;
			out << YAML::EndMap;
		}
		if (entity.HasComponent<AQCameraComponent>())
		{
			out << YAML::Key << "CameraComponent";
			out << YAML::BeginMap;

			auto component = entity.GetComponent<AQCameraComponent>();
			auto camera = component->Camera;

			out<<YAML::Key << "Camera" << YAML::Value;
				out << YAML::BeginMap;
				out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
				out << YAML::Key << "PerspectiveFov" << YAML::Value << camera.GetPerspectiveFov();
				out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNear();
				out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFar();
				out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
				out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNear();
				out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFar();
				out << YAML::Key << "AspectRatio" << YAML::Value << camera.GetAspectRatio();
				out << YAML::EndMap;
			out << YAML::Key << "IsPrimary" << YAML::Value << component->IsPrimary;
			out << YAML::Key << "FixedAspectRatio" << YAML::Value << component->FixedAspectRatio;
			
			out << YAML::EndMap;
		}
		if (entity.HasComponent<AQSpiriteComponent>())
		{
			out << YAML::Key << "ColorComponent";
			out << YAML::BeginMap;
			auto component = entity.GetComponent<AQSpiriteComponent>();
			out << YAML::Key << "Color" << YAML::Value << component->Color;

			out << YAML::EndMap;
		}
		out << YAML::EndMap;
	}

	void DeserializeComponentsToEntity(AQEntity& entity, YAML::detail::iterator_value& entitydata)
	{
		//TransformComponent
		auto transformcomponent = entitydata["TransformComponent"];
		if (transformcomponent)
		{
			auto component = entity.GetComponent<AQTransformComponent>();
			
			component->Translation = transformcomponent["Translation"].as<Eigen::Vector3f>();
			component->Rotation = transformcomponent["Rotation"].as<Eigen::Vector3f>();
			component->Scale = transformcomponent["Scale"].as<Eigen::Vector3f>();
		}
		//_________________________________________________________
		//CameraComponent
		auto cameracomponent = entitydata["CameraComponent"];
		if (cameracomponent)
		{
			auto component = entity.AddComponent<AQCameraComponent>();
			auto& camera = component->Camera;
			auto cameraprops = cameracomponent["Camera"];
			camera.SetProjectionType((AQSceneCamera::ProjectionType)cameraprops["ProjectionType"].as<int>());
			camera.SetPerspectiveFov(cameraprops["PerspectiveFov"].as<float>());
			camera.SetPerspectiveNear(cameraprops["PerspectiveNear"].as<float>());
			camera.SetPerspectiveFar(cameraprops["PerspectiveFar"].as<float>());
			camera.SetOrthographicSize(cameraprops["OrthographicSize"].as<float>());
			camera.SetOrthographicNear(cameraprops["OrthographicNear"].as<float>());
			camera.SetOrthographicFar(cameraprops["OrthographicFar"].as<float>());
			camera.SetAspectRatio(cameraprops["AspectRatio"].as<float>());
			component->IsPrimary = cameracomponent["IsPrimary"].as<bool>();
			component->FixedAspectRatio = cameracomponent["FixedAspectRatio"].as<bool>();
		}
		//_________________________________________________________
		//ColorComponent
		auto colorcomponent = entitydata["ColorComponent"];
		if (colorcomponent)
		{
			auto component = entity.AddComponent<AQSpiriteComponent>();
			component->Color = colorcomponent["Color"].as<Eigen::Vector4f>();
		}
		//_________________________________________________________

	 }
}