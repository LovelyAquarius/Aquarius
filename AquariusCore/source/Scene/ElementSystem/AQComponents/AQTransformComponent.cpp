#include "AQPCH.h"
#include "AQTransformComponent.h"



namespace Aquarius
{



	AQRef<AQTransformComponent> AQTransformComponent::Create()
	{
		return new AQTransformComponent();
	}


	AQRef<AQTransformComponent> AQTransformComponent::Create(const Eigen::Vector3f& translation, const std::string name)
	{
		return new AQTransformComponent(translation, name);
	}

	AQRef<AQTransformComponent> AQTransformComponent::Create(const Eigen::Vector3f& translation)
	{
		return new AQTransformComponent(translation);
	}


	AQRef<AQObject> AQTransformComponent::Copy()
	{
		return new AQTransformComponent(*this);
	}

	Eigen::Matrix4f AQTransformComponent::GetTransform() const
	{
		Eigen::Transform<AQFLOAT, 3, Eigen::Affine> transform;
		transform.setIdentity();
		transform.translate(Translation);
		auto eulerangle =
			Eigen::AngleAxis<AQFLOAT>(Rotation.x(), Eigen::Vector3f::UnitX()) *
			Eigen::AngleAxis<AQFLOAT>(Rotation.y(), Eigen::Vector3f::UnitY()) *
			Eigen::AngleAxis<AQFLOAT>(Rotation.z(), Eigen::Vector3f::UnitZ());
		transform.rotate(eulerangle);
		transform.scale(Scale);
		//AQ_Scale_Self(transform.matrix(), Scale);
		//transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(Rotation.x()), Eigen::Vector3f(1.0f, 0.0f, 0.0f)));
		//transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(Rotation.y()), Eigen::Vector3f(0.0f, 1.0f, 0.0f)));
		//transform.rotate(Eigen::AngleAxis<AQFLOAT>(AQ_DegreeToRadian(Rotation.z()), Eigen::Vector3f(0.0f, 0.0f, 1.0f)));

		return transform.matrix();
	}



	AQTransformComponent::AQTransformComponent()
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQTransformComponent);
	}

	AQTransformComponent::AQTransformComponent(const AQTransformComponent& other)
		:Translation(other.Translation), Rotation(other.Rotation), Scale(other.Scale)
	{
		AQ_COPY_AQOBJECT_NAME_AND_TYPE(other);
	}

	AQTransformComponent::AQTransformComponent(const Eigen::Vector3f& translation, const std::string& name)
		: Translation(translation)
	{
		AQ_INITIALIZE_AQOBJECT_NAME_AND_TYPE(name, AQTransformComponent);
	}

	AQTransformComponent::AQTransformComponent(const Eigen::Vector3f& translation)
		: Translation(translation)
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQTransformComponent);
	}


}