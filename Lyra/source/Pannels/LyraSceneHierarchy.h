#pragma once
#include "core/AquariusCore.h"
#include "core/Layer.h"
#include "Scene/AQEntity.h"

namespace Aquarius
{
	class LyraSceneHierarchy:public Layer
	{
		friend class AQScene;
	public: 
		LyraSceneHierarchy(const AQRef<AQScene>& scene);

		virtual void OnImGuiRender()override;
	public:

		void SetContext(const AQRef<AQScene>& scene);
	private:
		void DrawEntityNode(AQEntity entity);
		void DrawProperties(AQEntity entity);
	private:
		AQRef<AQScene> m_Scene;
		AQEntity m_Selection;
	};





}


