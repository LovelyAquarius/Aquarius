#include "LyraPCH.h"
#include"Aquarius.h"
#include "LyraContentBrowser.h"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <IMGUI/imgui_internal.h>
#include <IconsFontAwesome/IconsFontAwesome6.h>
namespace Aquarius
{
	static const std::filesystem::path assetsdirectoty = "source\\Assets\\Internal";
	LyraContentBrowser::LyraContentBrowser()
		:Layer("LyraContentBrowser"), m_CurrentDirectory(assetsdirectoty)
	{
		SetTranslation(Translation::English);
		m_DirectoryIcon = AQTexture2D::Create("Resources/Icons/ContentBrowser/DirectoryIcon.png");
		m_FileIcon = AQTexture2D::Create("Resources/Icons/ContentBrowser/FileIcon.png");
	}

	static std::string Tranlation_Content_Browser;

	void LyraContentBrowser::SetTranslation(const Translation language)
	{
		m_Translaion = language;

		if (m_Translaion == Translation::English)
		{
			Tranlation_Content_Browser = AQ_TRANSLATION_ENGLISH_Content_Browser;
		}
		else if (m_Translaion == Translation::Chinese)
		{
			Tranlation_Content_Browser = AQ_TRANSLATION_CHINESE_Content_Browser;
		}
		
	}


	

	void LyraContentBrowser::OnImGuiRender()
	{
		ImGui::Begin(Tranlation_Content_Browser.c_str());//创建不同名字的窗口不能记录之前的状态

		if (m_CurrentDirectory != assetsdirectoty)
		{
			if (ImGui::Button(ICON_U_F190))
			{
				m_CurrentDirectory = m_CurrentDirectory.parent_path();
			}
		}


		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);




		for (auto& directory : std::filesystem::directory_iterator(m_CurrentDirectory))//这里扫描是每一帧执行的，之后可考虑性能
		{
			const auto& path = directory.path();
			auto relativepath = std::filesystem::relative(path, assetsdirectoty);
			std::string filename = relativepath.filename().string();

			if (directory.is_directory())
			{
				if (ImGui::Button(filename.c_str()))
				{
					m_CurrentDirectory /= path.filename();
				}
			}
		}


		ImGui::End();

	}







}