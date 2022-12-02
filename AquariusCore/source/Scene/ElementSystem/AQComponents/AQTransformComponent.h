#pragma once
#include "Scene/ElementSystem/AQComponent.h"
#include "MathSystem/AQMathEx.h"

namespace Aquarius
{

	class AQUARIUS_API AQTransformComponent : public AQComponent
	{
	public:
		static AQRef<AQTransformComponent> Create();
		static AQRef<AQTransformComponent> Create(const Eigen::Vector3f& translation, const std::string name);
		static AQRef<AQTransformComponent> Create(const Eigen::Vector3f& translation);



	public:
		virtual AQRef<AQObject> Copy();
		Eigen::Matrix4f GetTransform() const;

	public:
		Eigen::Vector3f Translation = {0.0f,0.0f,0.0f};
		Eigen::Vector3f Rotation = { 0.0f,0.0f,0.0f };
		Eigen::Vector3f Scale = { 1.0f,1.0f,1.0f };
	protected:
		AQTransformComponent();
		AQTransformComponent(const AQTransformComponent& other);
		AQTransformComponent(const Eigen::Vector3f& translation,const std::string& name);
		AQTransformComponent(const Eigen::Vector3f& translation);

	private:

	};

}