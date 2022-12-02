#pragma once
#include "Renderer/AQUniformBuffer.h"
#include "MathSystem/AQMathEx.h"

namespace Aquarius
{
	class CommonCamera2DData :public AQObject
	{
	public:
		struct CameraData
		{
			AQFLOAT ViewProjection[16];
		};

	public:
		static AQRef<CommonCamera2DData> Create();
	public:
		CameraData Data;
		AQRef<AQUniformBuffer> CameraUniformBuffer;
	public:
		void SetData(const Eigen::Matrix4f& cameravp);

	protected:
		CommonCamera2DData();
		CommonCamera2DData(const CommonCamera2DData& other) = delete;
	};


}


