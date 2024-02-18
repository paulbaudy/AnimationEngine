#include "App.h"

#include <cassert>

namespace AN
{
	void FApp::Run()
	{
		bRunning = Init();

		// assert(false); TODO assert lib
		while (bRunning)
		{

		}

	}

	bool FApp::Init()
	{
		return true;
	}

}
