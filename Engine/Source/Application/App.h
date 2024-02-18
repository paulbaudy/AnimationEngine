#pragma once

#include "CoreMinimal.h"

#include "Window/Window.h"

namespace AN
{
	// Runnable application
	class FApp
	{
	public:
		FApp();

		void Run();
		bool Init();

		static FApp* Instance;
	private:
		std::unique_ptr<IWindow> MainWindow;
		bool bRunning = false;
	};

	FApp* MakeApp();
}