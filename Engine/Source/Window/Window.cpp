#include "Window.h"

#include "GLFW/glfw3.h"

namespace AN
{
	static void GLFWErrorCallback(int InError, const char* InDesc)
	{
		// todo log
	}

	std::unique_ptr<IWindow> IWindow::MakeWindow()
	{
		// TODO platform specific
		return std::make_unique<FGlfwWindow>();
	}

	FGlfwWindow::FGlfwWindow()
		: IWindow()
	{
		const float Width = 500.f;
		const float Height = 400.f;

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

	}
	void FGlfwWindow::Update()
	{
		glfwPollEvents();
		glfwSwapBuffers(Instance);
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

