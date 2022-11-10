#include "AQPCH.h"
#include "AQCameraComponent.h"


namespace Aquarius
{



	AQRef<AQCameraComponent> AQCameraComponent::Create(const Eigen::Matrix4f& projection, const std::string name)
	{
		return new AQCameraComponent(projection,name);
	}

	AQRef<AQCameraComponent> AQCameraComponent::Create(const Eigen::Matrix4f& projection)
	{
		return new AQCameraComponent(projection);
	}

	AQRef<AQCameraComponent> AQCameraComponent::Create()
	{
		return new AQCameraComponent();
	}

	AQRef<AQObject> AQCameraComponent::Copy()
	{
		return new AQCameraComponent(*this);
	}

	AQCameraComponent::AQCameraComponent(const AQCameraComponent& other)
		: Camera(other.Camera)
	{
		AQ_COPY_AQOBJECT_NAME_AND_TYPE(other);
	}

	AQCameraComponent::AQCameraComponent(const Eigen::Matrix4f& projection, const std::string& name)
		: Camera(projection)
	{
		AQ_INITIALIZE_AQOBJECT_NAME_AND_TYPE(name, AQColorComponent);
	}

	AQCameraComponent::AQCameraComponent(const Eigen::Matrix4f& projection)
		: Camera(projection)
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQCameraComponent);
	}


}