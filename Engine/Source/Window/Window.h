#pragma once

#include <memory>

struct GLFWwindow;

namespace AN
{
	class IWindow
	{
	public:
		// TODO seperate entry point from context windows
		static std::unique_ptr<IWindow> MakeWindow();

		virtual void Update() = 0;
		virtual void SetVSyncEnabled(bool bEnabled) = 0;

	protected:
		IWindow()
		{
		}

		bool bVSync = true;
	};

	class FGlfwWindow : public IWindow
	{
	public:
		FGlfwWindow();

		// Begin IWindow interface
		virtual void Update() override;
		virtual void SetVSyncEnabled(bool bEnabled) override;
		// ~End IWindow interface

	private:
		GLFWwindow* Instance;

	};
}