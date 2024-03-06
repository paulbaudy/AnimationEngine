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
		ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Transform Component", tree_node_flags))
		{
			FTransformComponent& Transform = Scene->GetRegistry().get<FTransformComponent>(InEntity->GetHandle());
			ImGui::DragFloat3("Translation", &Transform.Translation.x);
			ImGui::DragFloat3("Rotation", &Transform.RotEuler.x);
			ImGui::DragFloat3("Scale", &Transform.Scale.x);


			ImGui::TreePop();
		}

		if (FMeshComponent* MeshComp = InEntity->GetComponent<FMeshComponent>())
		{
			if (ImGui::TreeNodeEx("Mesh Component", tree_node_flags))
			{
				ImGui::Checkbox("Draw", &MeshComp->bDraw);
				ImGui::TreePop();
			}
		}
	}
}
