#pragma once
#include "Scene/AQScriptableEntity.h"

namespace Aquarius
{
	class AQUARIUS_API AQCameraController:public AQScriptableEntity
	{

	//本地脚本通用方法
	public:
		virtual void OnCreate()override;
		virtual void OnDestroy()override;
		virtual void OnUpdate(DeltaTime dt)override;
	public:
		AQFLOAT m_CameraMovingSpeed = 1.0f;
		AQFLOAT m_CameraRotationSpeed = 10.0f;//单位degree

	};



}


