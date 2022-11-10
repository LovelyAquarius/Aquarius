#include "AQPCH.h"
#include "AQNativeScriptComponent.h"
namespace Aquarius
{




	AQRef<AQNativeScriptComponent> AQNativeScriptComponent::Create()
	{
		return new AQNativeScriptComponent();
	}

	AQRef<AQNativeScriptComponent> AQNativeScriptComponent::Create(std::string name)
	{
		return new AQNativeScriptComponent(name);
	}

	AQNativeScriptComponent::AQNativeScriptComponent()
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(AQNativeScriptComponent);
	}

	AQNativeScriptComponent::AQNativeScriptComponent(std::string& name)
	{
		AQ_INITIALIZE_AQOBJECT_NAME_AND_TYPE(name, AQNativeScriptComponent);
	}

}
