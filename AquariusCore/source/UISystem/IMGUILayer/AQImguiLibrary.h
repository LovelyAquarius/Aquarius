#pragma once
#include<IMGUI/imgui.h>
#define IMGUI_DEFINE_MATH_OPERATORS
#include<imgui_internal.h>
namespace Aquarius
{
     bool  AQImgui_TreeNodeEx(const char* label, ImGuiTreeNodeFlags flags = 0);
     bool  AQImgui_TreeNodeEx(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
     bool  AQImgui_TreeNodeEx(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, ...) IM_FMTARGS(3);
     bool  AQImgui_TreeNodeExV(const char* str_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);
     bool  AQImgui_TreeNodeExV(const void* ptr_id, ImGuiTreeNodeFlags flags, const char* fmt, va_list args) IM_FMTLIST(3);

	bool AQImgui_TreeNodeBehavior(ImGuiID id, ImGuiTreeNodeFlags flags, const char* label, const char* label_end);
    void AQImgui_Image(ImTextureID user_texture_id, const ImVec2& size, const ImVec2& offset = ImVec2(0, 0), const ImVec2& uv0 = ImVec2(0, 0), const ImVec2& uv1 = ImVec2(1, 1), const ImVec4& tint_col = ImVec4(1, 1, 1, 1), const ImVec4& border_col = ImVec4(0, 0, 0, 0));
   







}

