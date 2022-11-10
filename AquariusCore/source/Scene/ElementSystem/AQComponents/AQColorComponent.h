#pragma once
#include "Scene/ElementSystem/AQComponent.h"
#include "MathSystem/AQEigen.h"

namespace Aquarius
{
	class AQColorComponent :public AQComponent
	{
	public:
		static AQRef<AQColorComponent> Create();
		static AQRef<AQColorComponent> Create(const Eigen::Vector4f& color, const std::string name);
		static AQRef<AQColorComponent> Create(const Eigen::Vector4f& color);
	public:
		Eigen::Vector4f Color;

	public:
		virtual AQRef<AQObject> Copy();
		operator const Eigen::Vector4f& ()const;
		operator Eigen::Vector4f();
	protected:
		AQColorComponent();
		AQColorComponent(const AQColorComponent& other);
		AQColorComponent(const Eigen::Vector4f& color, const std::string& name);
		AQColorComponent(const Eigen::Vector4f& color);

	private:


	};


}
