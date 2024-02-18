
#pragma once

#include "Application/App.h"

extern AN::FApp* AN::MakeApp();

int main(void)
{
	AN::FApp* App = AN::MakeApp();
	App->Run();

	delete App;
	return 0;
}
