#include "Entity.h"

#include "Core/Components.h"
#include "UI/ComponentDescriptors.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/Shader.h"

#include <imgui.h>
#include <glad/glad.h>



namespace AN
{
	FScene::FScene()
	{

	}

	void FScene::Init()
	{
		Mat.PushShader("Content/Shaders/Unlit.vert", GL_VERTEX_SHADER);
		Mat.PushShader("Content/Shaders/Unlit.frag", GL_FRAGMENT_SHADER);
		
		Buffer.Vertices = {
		   -1.0f, -1.0f, 0.0f,
		   1.0f, -1.0f, 0.0f,
		   0.0f,  1.0f, 0.0f,
		};
	}

	void FScene::AddEntity()
	{
		FEntity NewEntity(EntityRegistry.create(), this);

		// Add transform component by default. All entities are scene components by design
		NewEntity.AddComponent<FTransformComponent>();

		Entities.push_back(NewEntity);
	}

	void FScene::Update()
	{
	}


	void FScene::Render()
	{
	}

	void FScene::DrawEntities()
	{
		ImGui::Begin("Entities");
		static ImGuiTableFlags flags = ImGuiTableFlags_BordersV | ImGuiTableFlags_BordersOuterH | ImGuiTableFlags_Resizable | ImGuiTableFlags_RowBg | ImGuiTableFlags_NoBordersInBody;

		static ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_Selected;

		//HelpMarker("See \"Columns flags\" section to configure how indentation is applied to individual columns.");
		if (ImGui::BeginTable("3ways", 3, flags))
		{
			// The first column will use the default _WidthStretch when ScrollX is Off and _WidthFixed when ScrollX is On
			ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_NoHide);
			ImGui::TableSetupColumn("Size", ImGuiTableColumnFlags_WidthFixed, 16.f * 12.0f);
			ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed, 16.f * 18.0f);
			ImGui::TableHeadersRow();

			for (auto& InEntity : Entities)
			{
				DrawEntity(InEntity);
			}

			ImGui::EndTable();
		}
		ImGui::End();

		ImGui::Begin("Details");
		FEntity* Selected = GetSelectedEntity();
		if (nullptr != Selected)
		{
			ImGui::InputText("##", Selected->GetLabel().data(), 32);
			AN:DrawComponents(Selected);
		}
		ImGui::End();

		
	}
	void FScene::DrawEntity(FEntity& InEntity)
	{
		ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_OpenOnArrow;
		if (SelectedEntityId == InEntity.GetID())
			tree_node_flags |= ImGuiTreeNodeFlags_Selected;

		ImGui::TableNextRow();
		ImGui::TableNextColumn();

		bool bOpen = ImGui::TreeNodeEx(InEntity.GetLabel().c_str(), tree_node_flags);
		if (ImGui::IsItemClicked())
		{
			SelectedEntityId = InEntity.GetID();
		}

		ImGui::TableNextColumn();
		ImGui::TextDisabled("--");
		ImGui::TableNextColumn();
		ImGui::TextUnformatted("Entity");


		if (bOpen)
		{
			// todo display childs 
			ImGui::TreePop();
		}
	}
	void FScene::DrawViewport()
	{
		ImGui::Begin("Viewport");
		//FrameBuffer.Bind();

		static FFrameBuffer b(100.f, 100.f);
		b.Bind();
		auto size = ImGui::GetContentRegionAvail();
		// @todo move these on resize
		b.RescaleFrameBuffer(size.x, size.y);
		glViewport(0, 0, size.x, size.y);

		// Draw entities

		Mat.Bind();
		Buffer.BindBuffers();

		static float NearClipPlane = 0.01f;
		static float FarClipPlane = 100.f;
		float Aspect = size.x / size.y;

		glm::mat4 Projection = glm::perspective(glm::radians(EditorCamera.FOV), Aspect, NearClipPlane, FarClipPlane);
		FTransformComponent ViewComp;
		ViewComp.Translation.z = -5.f;
		glm::mat4 ViewMat = ViewComp.ToMat();

		auto ComponentsView = EntityRegistry.view<FTransformComponent>();
		for (const auto& Component : ComponentsView)
		{
			auto [Transform] = ComponentsView.get(Component);
			glm::mat4 Model = Transform.ToMat();

			Mat.SetMat4("projection", &Projection[0][0]);
			Mat.SetMat4("view", &ViewMat[0][0]);
			Mat.SetMat4("model", &Model[0][0]);
			Buffer.Draw();
		}

		Mat.Unbind();
		Buffer.UnbindBuffers();

		ImGui::Image(
			(ImTextureID)b.getFrameTexture(),
			ImGui::GetContentRegionAvail(),
			ImVec2(0, 1),
			ImVec2(1, 0));

		b.Unbind();

		ImGui::End();
	}
}

