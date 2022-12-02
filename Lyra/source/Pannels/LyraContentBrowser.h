#pragma once
#include "core/AquariusCore.h"
#include "core/Layer.h"
#include "Renderer/AQTexture.h"
#include "core/Translation.h"
#include <filesystem>

namespace Aquarius
{
	class LyraContentBrowser:public Layer
	{
	public:
		LyraContentBrowser();
		virtual void OnImGuiRender()override;
		void SetTranslation(const Translation language);
	public:
		
	private:
		Translation m_Translaion = Translation::English;
		std::filesystem::path m_CurrentDirectory;
		AQRef<AQTexture2D> m_DirectoryIcon;
		AQRef<AQTexture2D> m_FileIcon;
	};

}