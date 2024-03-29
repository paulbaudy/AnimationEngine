#pragma once

#include "Window/Window.h"
#include "Core/Event.h"
#include "Core/Core.h"

namespace AN
{
	class IWindow;

	// Runnable application
	class FApp : public IEventVisitor
	{
	public:
		FApp();

		void Run();
		bool Init();

		static FApp* Instance;
	protected: 
		// Begin IEventListener Interface
		void OnEvent(const FWindowCloseEvent& InEvent) override final;
		void OnEvent(const FMouseMoveEvent& InEvent) override final;
		void OnEvent(const FKeyEvent& InEvent) override final;
		// ~End IEventListener Interface
	private:
		TUniquePtr<IWindow> MainWindow;
		bool bRunning = false;
	};

	FApp* MakeApp();
}