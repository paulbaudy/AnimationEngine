#pragma once

#include "Core/Core.h"
#include "Core/Event.h"
#include "Core/Entity.h"

#include <functional>

struct GLFWwindow;

namespace AN
{
	class IWindow
	{
	public:
		// TODO seperate entry point from context windows
		static TUniquePtr<IWindow> MakeWindow();

		virtual void Update() = 0;
		virtual void Render() = 0;
		virtual void SetVSyncEnabled(bool bEnabled) = 0;
		virtual void SetCallback(std::function<void(const FEvent&)> InCallback) = 0;

		// Events
		virtual void OnMouseMoved(double xpos, double ypos) {}
		virtual void OnKey(int,int,int,int) {}

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

	public:
		// Begin IWindow interface
		virtual void Update() override;
		virtual void Render() override;
		virtual void SetCallback(std::function<void(const FEvent&)> InCallback) override;
		virtual void OnMouseMoved(double xpos, double ypos) override;
		virtual void OnKey(int, int, int, int) override;
		virtual void SetVSyncEnabled(bool bEnabled) override;
		// ~End IWindow interface

		static GLFWwindow* GetInstance() { return gInstance; };
		static GLFWwindow* gInstance;


	private:
		GLFWwindow* Instance;
		FScene Scene;

		bool bShowStyleEditor = false;
		bool bShowImGuiDemo = false;
		
		struct FGlfwUserData
		{
			std::function<void(const FEvent&)> EventCallback;
		};
		FGlfwUserData UserData;
	};
}