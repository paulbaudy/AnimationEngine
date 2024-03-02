#include "Window.h"

#include "Core/Log.h"

#include "GLFW/glfw3.h"

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <glad/glad.h>

namespace AN
{
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


		Instance = glfwCreateWindow((int)Width, (int)Height, "Animation Engine", nullptr, nullptr);
		glfwSetWindowUserPointer(Instance, &UserData);


		// Binding callbacks
		glfwSetWindowCloseCallback(Instance, [](GLFWwindow* window)
		{
			FGlfwUserData& Data = *(FGlfwUserData*)glfwGetWindowUserPointer(window);

			FWindowCloseEvent Event;
			Data.EventCallback(Event);
		});

		glfwMakeContextCurrent(Instance);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

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

		glViewport(0, 0, Width, Height);

		FrameBuffer = FFrameBuffer(100.f, 100.f);
	}

	void FGlfwWindow::Update()
	{
		//FrameBuffer.Bind();




		//FrameBuffer.Unbind();
	}

	void FGlfwWindow::Render()
	{
		// -- Init update --


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

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{


				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::Begin("Entities");
		ImGui::End();

		ImGui::Begin("Details");
		ImGui::End();

		ImGui::Begin("Viewport");
			//FrameBuffer.Bind();

			static FFrameBuffer b(100.f, 100.f);
			b.Bind();
			auto size = ImGui::GetContentRegionAvail();
			b.RescaleFrameBuffer(size.x, size.y);

			glViewport(0, 0, size.x, size.y);

			// An array of 3 vectors which represents 3 vertices
			static const GLfloat g_vertex_buffer_data[] = {
			   -1.0f, -1.0f, 0.0f,
			   1.0f, -1.0f, 0.0f,
			   0.0f,  1.0f, 0.0f,
			};

			// This will identify our vertex buffer
			GLuint vertexbuffer;
			// Generate 1 buffer, put the resulting identifier in vertexbuffer
			glGenBuffers(1, &vertexbuffer);
			// The following commands will talk about our 'vertexbuffer' buffer
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			// Give our vertices to OpenGL.
			glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

			// 1st attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
			glDisableVertexAttribArray(0);

			ImGui::Image(
				(ImTextureID)b.getFrameTexture(),
				ImGui::GetContentRegionAvail(),
				ImVec2(0, 1),
				ImVec2(1, 0));

			//FrameBuffer.Unbind();
			b.Unbind();

		ImGui::End();

		ImGui::Begin("Log");
		ImGui::End();

		ImGui::Begin("Stats");
		ImGui::End();


		ImGui::End();
		//ImGui::PopStyleVar();






		//ImGui::Render();
		//ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		//ImGui::UpdatePlatformWindows();

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

		glfwPollEvents();
		glfwSwapBuffers(Instance);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		FrameBuffer.Unbind();
	}

	void FGlfwWindow::SetCallback(std::function<void(const FEvent&)> InCallback)
	{
		UserData.EventCallback = InCallback;
	}

	void FGlfwWindow::SetVSyncEnabled(bool bEnabled)
	{
		bVSync = bEnabled;
		glfwSwapInterval((int)bVSync);
	}
}

