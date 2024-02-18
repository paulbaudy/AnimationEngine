
#include "Core/Main.h"

// Editor entry point
namespace AN
{
	class FEditorApp : public FApp
	{
	public:
		FEditorApp()
			: FApp()
		{
			LOG_TRACE("Building editor application")

		}
	};

	FApp* MakeApp()
	{
		return new FEditorApp();
	}
}