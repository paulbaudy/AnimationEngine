#pragma once

#include "Core/Core.h"
#include "Core/Event.h"
#include "Core/Entity.h"
#include "Rendering/FrameBuffer.h"

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
		virtual void SetVSyncEnabled(bool bEnabled) override;
		// ~End IWindow interface

	private:
		GLFWwindow* Instance;
		FFrameBuffer FrameBuffer;
		FScene Scene;
		
		struct FGlfwUserData
		{
			std::function<void(const FEvent&)> EventCallback;
		};
		FGlfwUserData UserData;
	};
}