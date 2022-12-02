#include "AQPCH.h"
#include "CommonCamera2DData.h"
namespace Aquarius
{
	AQRef<CommonCamera2DData> CommonCamera2DData::Create()
	{
		return new CommonCamera2DData();
	}
	void CommonCamera2DData::SetData(const Eigen::Matrix4f& cameravp)
	{
		for (int i = 0; i < 16; i++)
		{
			Data.ViewProjection[i] = cameravp.data()[i];
		}
		CameraUniformBuffer->SetData(&Data, sizeof(CameraData));
	}

	CommonCamera2DData::CommonCamera2DData()
	{
		AQ_INITIALIZE_AQOBJECT_TYPE(CommonCamera2DData);
		CameraUniformBuffer = AQUniformBuffer::Create("Render2D CameraUniformBuffer", sizeof(CameraData), 0);
	}


}