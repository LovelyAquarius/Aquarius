#pragma once
#pragma once
#include "core/AquariusCore.h"
#include "Renderer/RenderAPI.h"

namespace Aquarius
{
	class AQUARIUS_API OpenGLRenderAPI :public RenderAPI
	{
	public:

		virtual void SetClearcolor(const glm::vec4& color) override;
		virtual void Clear()override;
		virtual void DrawElement(const std::shared_ptr<AQVertexArray>& VAO, uint32_t elementlocation = 0)override;






	};


}