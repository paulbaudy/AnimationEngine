#include "Window.h"

#include "Core/Log.h"

#include "GLFW/glfw3.h"

#include <imgui.h>
#include "UI/ImGuiFactory.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <imgui_internal.h>


namespace AN
{
	GLFWwindow* FGlfwWindow::gInstance = nullptr;

	static void GLFWErrorCallback(int InError, const char* InDesc)
	{
		// todo log
	}

	TUniquePtr<IWindow> IWindow::MakeWindow()
	{
		// TODO platform specific
		return MakeUnique<FGlfwWindow>();
	}

	FGlfwWindow::FGlfwWindow()
		: IWindow()
	{
		LOG_TRACE("Building GLFW Window")

		const float Width = 1000.f;
		const float Height = 800.f;

		int bSuccess = glfwInit();
		if (!bSuccess)
		{
			//todo log
			exit(1);
		}

		glfwSetErrorCallback(GLFWErrorCallback);
		// GL 3.2 + GLSL 150
		const char* glsl_version = "#version 330";
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
		glfwWindowHint(GLFW_MAXIMIZED, GLFW_TRUE);

		Instance = glfwCreateWindow((int)Width, (int)Height, "Animation Engine", nullptr, nullptr);
		gInstance = Instance;
		glfwSetWindowUserPointer(Instance, &UserData);


		// Binding callbacks
		glfwSetWindowCloseCallback(Instance, [](GLFWwindow* window)
		{
			FGlfwUserData& Data = *(FGlfwUserData*)glfwGetWindowUserPointer(window);

			FWindowCloseEvent Event;
			Data.EventCallback(Event);
		});
		glfwSetCursorPosCallback(Instance, [](GLFWwindow* window, double xpos, double ypos)
		{
			FGlfwUserData& Data = *(FGlfwUserData*)glfwGetWindowUserPointer(window);

			FMouseMoveEvent Event;
			Event.xpos = xpos;
			Event.ypos = ypos;

			Data.EventCallback(Event);
		});
		glfwSetKeyCallback(Instance, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			FGlfwUserData& Data = *(FGlfwUserData*)glfwGetWindowUserPointer(window);

			FKeyEvent Event;
			Event.key = key;
			Event.scancode = scancode;
			Event.action = action;
			Event.mods = mods;

			Data.EventCallback(Event);
		});

		glfwMakeContextCurrent(Instance);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

		AN::InitImGui(Instance);

		glViewport(0, 0, Width, Height);

		// Enable depth test
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		// Enable transparancy
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glfwWindowHint(GLFW_SAMPLES, 4);
		glEnable(GL_MULTISAMPLE);

		LastFrameTime = glfwGetTime();
		OnFrameEnded();

		glfwSwapInterval(0);


		Scene.Init();
		Scene.AddEntity();
		Scene.AddEntity();
		Scene.AddEntity();
	}

	void FGlfwWindow::PreUpdate()
	{
		glfwPollEvents();
	}

	void FGlfwWindow::Update()
	{
		Scene.Update(FrameTime);
	}

	void FGlfwWindow::Render()
	{
		// -- Init update --
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		// -- Begin update --

		static bool dockspaceOpen = true;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (true)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			//ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		//ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;
		style.WindowPadding = ImVec2(0.f, 0.f);

		auto& colors = ImGui::GetStyle().Colors;

		colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.11f, 1.0f };

		// Headers
		colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Buttons
		colors[ImGuiCol_Button] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Frame BG
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Tabs
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		// Title
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		// Resize Grip
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

		// Scrollbar
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

		// Check Mark
		colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);

		// Slider
		colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{


				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem("ImGui Style Editor", nullptr, &bShowStyleEditor);
				ImGui::MenuItem("ImGui Demo", nullptr, &bShowImGuiDemo);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		Scene.DrawEntities();
		Scene.DrawViewport();
		Scene.DrawSceneSettings();

		ImGui::Begin("Log");
		ImGui::End();

		ImGui::Begin("Stats");
		ImGui::Text("Frame Time: (%g)", FrameTime);
		ImGui::End();

		if (bShowStyleEditor)
		{
			ImGui::Begin("ImGui Style Editor", &bShowStyleEditor);
			ImGui::ShowStyleEditor();
			ImGui::End();
		}

		if (bShowImGuiDemo)
		{
			ImGui::Begin("ImGui Demo", &bShowImGuiDemo);
			ImGui::ShowDemoWindow();
			ImGui::End();
		}
		ImGui::End();


		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwSwapBuffers(Instance);
	}

	void FGlfwWindow::SetCallback(std::function<void(const FEvent&)> InCallback)
	{
		UserData.EventCallback = InCallback;
	}

	void FGlfwWindow::OnMouseMoved(double xpos, double ypos)
	{
		Scene.OnMouseMove(xpos, ypos);
	}

	void FGlfwWindow::OnKey(int key, int scancode, int action, int mods)
	{
		Scene.OnKey(key, scancode, action, mods);
	}

	void FGlfwWindow::OnFrameEnded()
	{
		const float Time = (float)glfwGetTime();
		FrameTime = Time - LastFrameTime;
		LastFrameTime = Time;
	}

	void FGlfwWindow::SetVSyncEnabled(bool bEnabled)
	{
		bVSync = bEnabled;
		glfwSwapInterval((int)bVSync);
	}
}

