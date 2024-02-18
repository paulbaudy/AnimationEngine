#pragma once

namespace AN
{
	// Runnable application
	class FApp
	{
	public:
		void Run();
		bool Init();
	private:
		bool bRunning = false;
	};

	FApp* MakeApp();
}