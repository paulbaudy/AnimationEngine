#pragma once

namespace AN
{
	class FEntity;

	// Can't constify because of ImGui
	void DrawComponents(FEntity* InEntity);
}