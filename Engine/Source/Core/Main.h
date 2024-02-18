
#pragma once

#include "Application/App.h"
#include "Core/Log.h"

extern AN::FApp* AN::MakeApp();

int main(void)
{
	// Todo log level as argument or define
	AN::FLogEngine::Init(SPDLOG_LEVEL_TRACE);

	LOG_TRACE("Building application from external entry point");

	AN::FApp* App = AN::MakeApp();
	App->Run();

	delete App;
	return 0;
}
