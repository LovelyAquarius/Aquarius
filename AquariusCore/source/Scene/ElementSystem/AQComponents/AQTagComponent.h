#pragma once
#include "Scene/ElementSystem/AQComponent.h"


namespace Aquarius
{
	class AQUARIUS_API AQTagComponent : public AQComponent
	{
	public:
		static AQRef<AQTagComponent> Create();
		static AQRef<AQTagComponent> Create(const std::string tagname);
		static AQRef<AQTagComponent> Create(const std::string tagname, const std::string name);
		
	public:
		virtual AQRef<AQObject> Copy();
		operator const std::string& ()const;
		operator std::string();
	public:
		std::string Tag;

	protected:
		AQTagComponent();
		AQTagComponent(AQTagComponent& other);
		AQTagComponent(const std::string& tagname);
		AQTagComponent(const std::string& tagname, const std::string& name);
	};



}


