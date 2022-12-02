#include "AQPCH.h"
#include "AQSpiriteComponent.h"

namespace Aquarius
{







	AQRef<AQSpiriteComponent> AQSpiriteComponent::Create()
	{
		return new AQSpiriteComponent();
	}

	AQRef<AQSpiriteComponent> AQSpiriteComponent::Create(const Eigen::Vector4f& color, const std::string name)
	{
		return new AQSpiriteComponent(color, name);
	}

	AQRef<AQSpiriteComponent> AQSpiriteComponent::Create(const Eigen::Vector4f& color)
	{
		return new AQSpiriteComponent(color);
	}

	AQRef<AQObject> AQSpiriteComponent::Copy()
	{
		return new AQSpiriteComponent(*this);
	}

	AQSpiriteComponent::operator const Eigen::Vector4f& () const
	{
		return Color;
	}

	AQSpiriteComponent::operator Eigen::Vector4f()
	{
		return Color;
	}

	AQSpiriteComponent::AQSpiriteComponent()
		:Color({1.0f,1.0f,1.0f,1.0f})
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQSpiriteComponent);
	}

	AQSpiriteComponent::AQSpiriteComponent(const AQSpiriteComponent& other)
		: Color(other.Color)
	{
		AQ_COPY_AQOBJECT_NAME_AND_TYPE(other);
	}

	AQSpiriteComponent::AQSpiriteComponent(const Eigen::Vector4f& color, const std::string& name)
		: Color(color)
	{
		AQ_INITIALIZE_AQOBJECT_NAME_AND_TYPE(name, AQSpiriteComponent);
	}

	AQSpiriteComponent::AQSpiriteComponent(const Eigen::Vector4f& color)
		: Color(color)
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQSpiriteComponent);
	}


}