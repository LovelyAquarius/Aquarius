#include "AQPCH.h"
#include "AQColorComponent.h"

namespace Aquarius
{







	AQRef<AQColorComponent> AQColorComponent::Create()
	{
		return new AQColorComponent();
	}

	AQRef<AQColorComponent> AQColorComponent::Create(const Eigen::Vector4f& color, const std::string name)
	{
		return new AQColorComponent(color, name);
	}

	AQRef<AQColorComponent> AQColorComponent::Create(const Eigen::Vector4f& color)
	{
		return new AQColorComponent(color);
	}

	AQRef<AQObject> AQColorComponent::Copy()
	{
		return new AQColorComponent(*this);
	}

	AQColorComponent::operator const Eigen::Vector4f& () const
	{
		return Color;
	}

	AQColorComponent::operator Eigen::Vector4f()
	{
		return Color;
	}

	AQColorComponent::AQColorComponent()
		:Color({1.0f,1.0f,1.0f,1.0f})
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQColorComponent);
	}

	AQColorComponent::AQColorComponent(const AQColorComponent& other)
		: Color(other.Color)
	{
		AQ_COPY_AQOBJECT_NAME_AND_TYPE(other);
	}

	AQColorComponent::AQColorComponent(const Eigen::Vector4f& color, const std::string& name)
		: Color(color)
	{
		AQ_INITIALIZE_AQOBJECT_NAME_AND_TYPE(name, AQColorComponent);
	}

	AQColorComponent::AQColorComponent(const Eigen::Vector4f& color)
		: Color(color)
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQColorComponent);
	}


}