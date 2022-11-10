#include "AQPCH.h"
#include "AQTransformComponent.h"



namespace Aquarius
{



	AQRef<AQTransformComponent> AQTransformComponent::Create()
	{
		return new AQTransformComponent();
	}


	AQRef<AQTransformComponent> AQTransformComponent::Create(const Eigen::Matrix4f& transform, const std::string name)
	{
		return new AQTransformComponent(transform, name);
	}

	AQRef<AQTransformComponent> AQTransformComponent::Create(const Eigen::Matrix4f& transform)
	{
		return new AQTransformComponent(transform);
	}


	AQRef<AQObject> AQTransformComponent::Copy()
	{
		return new AQTransformComponent(*this);
	}

	AQTransformComponent::operator const Eigen::Matrix4f& () const
	{
		return Transform;
	}

	AQTransformComponent::operator Eigen::Matrix4f()
	{
		return Transform;
	}

	AQTransformComponent::AQTransformComponent()
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQTransformComponent);
	}

	AQTransformComponent::AQTransformComponent(const AQTransformComponent& other)
		:Transform(other.Transform)
	{
		AQ_COPY_AQOBJECT_NAME_AND_TYPE(other);
	}

	AQTransformComponent::AQTransformComponent(const Eigen::Matrix4f& transform, const std::string& name)
		:Transform(transform)
	{
		AQ_INITIALIZE_AQOBJECT_NAME_AND_TYPE(name, AQTransformComponent);
	}

	AQTransformComponent::AQTransformComponent(const Eigen::Matrix4f& transform)
		:Transform(transform)
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQTransformComponent);
	}


}