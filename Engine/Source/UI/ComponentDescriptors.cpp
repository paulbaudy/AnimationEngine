#include "ComponentDescriptors.h"

#include <imgui.h>
#include "Core/Components.h"
#include "Core/Entity.h"
#include "Core/Scene.h"

namespace AN
{
	void DrawComponents(FEntity* InEntity)
	{
		FScene* Scene = InEntity->GetScene();

		if (ImGui::TreeNode("Transform Component"))
		{
			FTransformComponent& Transform = Scene->GetRegistry().get<FTransformComponent>(InEntity->GetHandle());
			ImGui::DragFloat3("Translation", &Transform.Translation.x);
			ImGui::DragFloat3("Rotation", &Transform.RotEuler.x);
			ImGui::DragFloat3("Scale", &Transform.Scale.x);


			ImGui::TreePop();
		}
	}
}
