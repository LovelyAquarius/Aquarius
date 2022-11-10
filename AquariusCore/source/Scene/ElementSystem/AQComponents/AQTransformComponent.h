#pragma once
#include "Scene/ElementSystem/AQComponent.h"
#include "MathSystem/AQEigen.h"

namespace Aquarius
{

	class AQUARIUS_API AQTransformComponent : public AQComponent
	{
	public:
		static AQRef<AQTransformComponent> Create();
		static AQRef<AQTransformComponent> Create(const Eigen::Matrix4f& transform, const std::string name);
		static AQRef<AQTransformComponent> Create(const Eigen::Matrix4f& transform);



	public:
		virtual AQRef<AQObject> Copy();
		operator const Eigen::Matrix4f& ()const;
		operator  Eigen::Matrix4f ();

	public:
		Eigen::Matrix4f Transform;

	protected:
		AQTransformComponent();
		AQTransformComponent(const AQTransformComponent& other);
		AQTransformComponent(const Eigen::Matrix4f& transform,const std::string& name);
		AQTransformComponent(const Eigen::Matrix4f& transform);

	private:

	};

}