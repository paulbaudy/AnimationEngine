#pragma once

#include "CoreMinimal.h"
#include "Window/Window.h"
#include "Core/Event.h"

namespace AN
{
	// Runnable application
	class FApp : public IEventVisitor
	{
	public:
		FApp();

		void Run();
		bool Init();

		static FApp* Instance;
	protected: 
		// BeginIEventListener Interface
		
		void OnEvent(const FWindowCloseEvent& InEvent) override final;
		// ~End IEventListener Interface
	private:
		std::unique_ptr<IWindow> MainWindow;
		bool bRunning = false;
	};

	FApp* MakeApp();
}