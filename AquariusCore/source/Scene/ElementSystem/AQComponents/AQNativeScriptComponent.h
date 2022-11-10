#pragma once
#include "core/TimeSystem.h"
#include "Scene/ElementSystem/AQComponent.h"
#include "Scene/AQScriptableEntity.h"
namespace Aquarius
{
	class AQNativeScriptComponent:public AQComponent
	{
	public:
		static AQRef<AQNativeScriptComponent> Create();
		static AQRef<AQNativeScriptComponent> Create(std::string name);

	public:
		AQScriptableEntity* Instance = nullptr;
		AQScriptableEntity* (*CreateScript)();
		void (*DestroyScript)(AQNativeScriptComponent*);
		
	public:


		template<typename ScriptableType>
			void Bind();
	protected:
		AQNativeScriptComponent();
		AQNativeScriptComponent(std::string& name);


	};



	template<typename ScriptableType>
	void AQNativeScriptComponent::Bind()
	{
		CreateScript = []() {return  static_cast<AQScriptableEntity*>(new ScriptableType()); };
		DestroyScript = [](AQNativeScriptComponent* nsc) {delete nsc->Instance; nsc->Instance = nullptr; };
	}

}


