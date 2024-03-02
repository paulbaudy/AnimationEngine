#pragma once

struct GLFWwindow;

namespace AN
{
	struct FFontSettings
	{
		std::string Name;
		std::string Path;
		float Size = 16.0f;
		bool bMerge = false;
	};

	void InitImGui(GLFWwindow* Instance);
	void AddFont(const FFontSettings& InSettings);
}