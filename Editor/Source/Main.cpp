
#include "Core/Core.h"

// Editor entry point
namespace AN
{
	class FEditorApp : public FApp
	{
	public:
		FEditorApp()
			: FApp()
		{

		}
	};

	FApp* MakeApp()
	{
		return new FEditorApp();
	}
}