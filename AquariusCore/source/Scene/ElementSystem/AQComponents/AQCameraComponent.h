#pragma once
#include "Scene/ElementSystem/AQComponent.h"
#include "Scene/AQSceneCamera.h"

namespace Aquarius
{
	class AQUARIUS_API AQCameraComponent :public AQComponent
	{
	public:
		static AQRef<AQCameraComponent> Create(const Eigen::Matrix4f& projection, const std::string name);
		static AQRef<AQCameraComponent> Create(const Eigen::Matrix4f& projection);
		static AQRef<AQCameraComponent> Create();
	public:
		AQSceneCamera Camera;
		AQBOOL IsPrimary = true;
		AQBOOL FixedAspectRatio = false;
	public:
		virtual AQRef<AQObject> Copy();

	protected:
		AQCameraComponent()=default;
		AQCameraComponent(const AQCameraComponent& other);
		AQCameraComponent(const Eigen::Matrix4f& projection, const std::string& name);
		AQCameraComponent(const Eigen::Matrix4f& projection);


	};



}
