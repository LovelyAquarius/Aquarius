#include "AQPCH.h"
#include "AQCameraController.h"
#include "core/Input.h"
#include "core/KeyCode.h"

#include "Scene/ElementSystem/AQComponents/AQTransformComponent.h"
#include "Scene/ElementSystem/AQComponents/AQTagComponent.h"
#include "Scene/ElementSystem/AQComponents/AQColorComponent.h"
#include "Scene/ElementSystem/AQComponents/AQCameraComponent.h"
#include "Scene/ElementSystem/AQComponents/AQNativeScriptComponent.h"



namespace Aquarius
{

	void AQCameraController::OnCreate()
	{
		AQ_CORE_INFO("AQCameraController Created");
	}

	void AQCameraController::OnDestroy()
	{
	}

	void AQCameraController::OnUpdate(DeltaTime dt)
	{
		auto& camera_transform = GetComponent<AQTransformComponent>()->Transform;

		//camera控制
		

		//if (m_RotationEnabled)
		//{
		//	if (Input::IsKeyPressed(AQ_KEY_Q))
		//	{
		//		m_CameraRotation += m_CameraRotationSpeed * dt;
		//		m_Camera.SetRotation(m_CameraRotation);
		//	}
		//	else if (Input::IsKeyPressed(AQ_KEY_E))
		//	{
		//		m_CameraRotation -= m_CameraRotationSpeed * dt;
		//		m_Camera.SetRotation(m_CameraRotation);
		//	}
		//}
		//___________________________________

	}


}