#include "App.h"

#include "Window/Window.h"
#include <cassert>

namespace AN
{
	FApp* FApp::Instance = nullptr;

	FApp::FApp()
	{
		MainWindow = IWindow::MakeWindow();
		//MainWindow->SetCallback(std::bind(&IEventVisitor::OnEvent, this, std::placeholders::_1));
		MainWindow->SetCallback([this](const FEvent& InEvent) { IEventVisitor::OnEvent(InEvent); });
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

	void FApp::OnEvent(const FWindowCloseEvent& InEvent)
	{
		bRunning = false;
	}


}
