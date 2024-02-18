#include "anpch.h"

#include "App.h"
#include "Core/Log.h"
#include "Window/Window.h"

namespace AN
{
	FApp* FApp::Instance = nullptr;

	FApp::FApp()
	{
		LOG_TRACE("Building core application")

		MainWindow = IWindow::MakeWindow();
		//MainWindow->SetCallback(std::bind(&IEventVisitor::OnEvent, this, std::placeholders::_1));
		MainWindow->SetCallback([this](const FEvent& InEvent) { IEventVisitor::OnEvent(InEvent); });
		FApp::Instance = this;
	}


	void FApp::Run()
	{
		bRunning = Init();

		LOG_TRACE("Running main engine loop")

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
		LOG_TRACE("OnEvent WindowCloseEvent")

		bRunning = false;
	}


}
