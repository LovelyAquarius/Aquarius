#pragma once
#include "Scene/ElementSystem/AQComponent.h"
#include "MathSystem/AQMathEx.h"

namespace Aquarius
{
	class AQSpiriteComponent :public AQComponent
	{
	public:
		static AQRef<AQSpiriteComponent> Create();
		static AQRef<AQSpiriteComponent> Create(const Eigen::Vector4f& color, const std::string name);
		static AQRef<AQSpiriteComponent> Create(const Eigen::Vector4f& color);
	public:
		Eigen::Vector4f Color;

	public:
		virtual AQRef<AQObject> Copy();
		operator const Eigen::Vector4f& ()const;
		operator Eigen::Vector4f();
	protected:
		AQSpiriteComponent();
		AQSpiriteComponent(const AQSpiriteComponent& other);
		AQSpiriteComponent(const Eigen::Vector4f& color, const std::string& name);
		AQSpiriteComponent(const Eigen::Vector4f& color);

	private:


	};


}
