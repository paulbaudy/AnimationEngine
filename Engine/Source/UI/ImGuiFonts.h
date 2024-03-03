#pragma once

namespace AN
{
	namespace ImGuiUtils
	{
		struct FFontSettings
		{
			std::string Name;
			std::string Path;
			float Size = 12.0f;
			bool bMerge = false;
		};

		void AddFont(const FFontSettings& InSettings);
	};
}