#include "anpch.h"
#include "ImGuiFactory.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <filesystem>

namespace AN
{
	void InitImGui(GLFWwindow* Instance)
	{
		// TODO move elsewhere
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

		ImGui_ImplGlfw_InitForOpenGL(Instance, true);
		ImGui_ImplOpenGL3_Init("#version 410");

		std::filesystem::path cwd = std::filesystem::current_path() / "test.txt";
		LOG_INFO("{0}", cwd.string());

		AddFont({"Roboto", "Content/Fonts/Roboto/Roboto-SemiMedium.ttf"});
	}

	void AddFont(const FFontSettings& InSettings)
	{
		ImFontConfig imguiFontConfig;
		imguiFontConfig.MergeMode = InSettings.bMerge;
		auto& io = ImGui::GetIO();
		ImFont* font = io.Fonts->AddFontFromFileTTF(InSettings.Path.data(), InSettings.Size, &imguiFontConfig, io.Fonts->GetGlyphRangesDefault());
	}
}

