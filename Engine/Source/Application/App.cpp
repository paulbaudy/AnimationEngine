#include "App.h"

#include "Window/Window.h"
#include <cassert>

namespace AN
{
	FApp* FApp::Instance = nullptr;

	FApp::FApp()
	{
		MainWindow = IWindow::MakeWindow();
		FApp::Instance = this;
	}


	void FApp::Run()
	{
		bRunning = Init();

		// assert(false); TODO assert lib
		while (bRunning)
		{

			MainWindow->Update();
		}

	}

	bool FApp::Init()
	{
		return true;
	}

}
