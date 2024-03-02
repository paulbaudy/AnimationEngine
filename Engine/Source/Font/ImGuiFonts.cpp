#include "anpch.h"
#include "ImGuiFonts.h"
#include <imgui.h>

namespace AN
{
	namespace ImGuiUtils
	{
		void AddFont(const FFontSettings& InSettings)
		{
			ImFontConfig imguiFontConfig;
			imguiFontConfig.MergeMode = InSettings.bMerge;
			auto& io = ImGui::GetIO();
			ImFont* font = io.Fonts->AddFontFromFileTTF(InSettings.Path.data(), InSettings.Size, &imguiFontConfig, io.Fonts->GetGlyphRangesDefault());
		}
	}
}


