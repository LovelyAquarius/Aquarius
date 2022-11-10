#include "AQPCH.h"
#include "AQTagComponent.h"
namespace Aquarius
{


	AQRef<AQTagComponent> AQTagComponent::Create(const std::string tagname, const std::string name)
	{
		return new AQTagComponent(tagname, name);
	}

	AQRef<AQTagComponent> AQTagComponent::Create()
	{
		return new AQTagComponent();
	}

	AQRef<AQTagComponent> AQTagComponent::Create(const std::string tagname)
	{
		return new AQTagComponent(tagname);
	}

	AQRef<AQObject> AQTagComponent::Copy()
	{
		return new AQTagComponent(*this);
	}

	AQTagComponent::operator const std::string& ()const
	{
		return Tag;
	}
	AQTagComponent::operator std::string()
	{
		return Tag;
	}




	AQTagComponent::AQTagComponent()
		:Tag("Unamed Tag")
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQTagComponent);
	}

	AQTagComponent::AQTagComponent(AQTagComponent& other)
		: Tag(other.Tag)
	{
		AQ_COPY_AQOBJECT_NAME_AND_TYPE(other);
	}

	AQTagComponent::AQTagComponent(const std::string& tagname)
		: Tag(tagname)
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQTagComponent);
		if (Tag == "")
			Tag = "Unamed Tag";
	}

	AQTagComponent::AQTagComponent(const std::string& tagname, const std::string& name)
		: Tag(tagname)
	{
		AQ_INITIALIZE_AQOBJECT_NAME_AND_TYPE(name, AQTagComponent);
		if (Tag == "")
			Tag = "Unamed Tag";
	}


}