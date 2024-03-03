#include "anpch.h"
#include "Entity.h"

#include "Core/Components.h"
#include "UI/ComponentDescriptors.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/Shader.h"

#include <imgui.h>
#include <glad/glad.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>

#include "GLFW/glfw3.h"
#include "Window/Window.h"

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

		GridBuffer.Vertices = { // Vertices
			1.f, 1.f, 0.f,
			-1.f, -1.f, 0.f,
			-1.f, 1.f, 0.f,
			-1.f, -1.f, 0.f,
			1.f, 1.f, 0.f,
			1.f, -1.f, 0.f,
		};

		GridMat.PushShader("Content/Shaders/Grid.vert", GL_VERTEX_SHADER);
		GridMat.PushShader("Content/Shaders/Grid.frag", GL_FRAGMENT_SHADER);


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

	void FScene::OnMouseMove(double xpos, double ypos)
	{
		if (!bCapturing)
		{
			MouseX = xpos;
			MouseY = ypos;
			return;
		}

		// todo expose delta time

		const double DeltaX = (xpos - MouseX);
		const double DeltaY = (ypos - MouseY);

		static float upAngle = 0.f;
		static float rightAngle = 0.f;

		upAngle += DeltaY * 0.1f;
		rightAngle += DeltaX * 0.1f;

		// todo investigate weird pitch values
		upAngle = std::clamp(upAngle, -90.f, 90.f);

		glm::quat q = glm::angleAxis(glm::radians(-rightAngle), glm::vec3(0, 1, 0)) ;
		q *= glm::angleAxis(glm::radians(-upAngle), glm::vec3(1, 0, 0));

		EditorCamera.Transform.SetQuat(q);

		MouseX = xpos;
		MouseY = ypos;
	}

	void FScene::OnKey(int key, int scancode, int action, int mods)
	{
		if (!bCapturing)
			return;

		glm::vec3 ForwardDir = glm::rotate(EditorCamera.Transform.GetQuat(), glm::vec3(0.f, 0.f, 1.f));
		glm::vec3 RightDir = glm::cross(ForwardDir, glm::vec3(0.f, 1.f, 0.f));

		EditorCamera.Transform.Translation -= ForwardDir * 0.1f;
		EditorCamera.Transform.Translation -= RightDir * 0.1f;
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
		auto& io = ImGui::GetIO();

		// Starting mouse capture
		if (!bCapturing && ImGui::IsWindowHovered() && ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{
			bCapturing = true;
			glfwSetInputMode(FGlfwWindow::GetInstance(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
		else if (bCapturing && !ImGui::IsMouseDown(ImGuiMouseButton_Right))
		{
			bCapturing = false;
			glfwSetInputMode(FGlfwWindow::GetInstance(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

		}

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
		FTransformComponent& ViewComp = EditorCamera.Transform;
		ViewComp.Translation.z = 5.f;
		ViewComp.Translation.y = 2.f;
		glm::mat4 ViewMat = glm::inverse(EditorCamera.Transform.ToMat());

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

		// Draw editor grid
		GridMat.Bind();
		GridBuffer.BindBuffers();
		FTransformComponent GridTransform;
		glm::mat4 Model = GridTransform.ToMat();

		GridMat.SetMat4("projection", &Projection[0][0]);
		GridMat.SetMat4("view", &ViewMat[0][0]);
		GridMat.SetMat4("model", &Model[0][0]);
		GridBuffer.Draw();

		GridMat.Unbind();
		GridBuffer.UnbindBuffers();


		ImGui::Image(
			(ImTextureID)b.getFrameTexture(),
			ImGui::GetContentRegionAvail(),
			ImVec2(0, 1),
			ImVec2(1, 0));

		b.Unbind();

		ImGui::End();
	}


	void FScene::DrawSceneSettings()
	{
		ImGui::Begin("Scene");

		ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns;

		if (ImGui::TreeNodeEx("Camera", tree_node_flags))
		{
			static ImGuiComboFlags flags = 0;

			//if (ImGui::BeginCombo("combo 1", combo_preview_value, flags))
			//{
			//	for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			//	{
			//		const bool is_selected = (item_current_idx == n);
			//		if (ImGui::Selectable(items[n], is_selected))
			//			item_current_idx = n;
			//
			//		// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			//		if (is_selected)
			//			ImGui::SetItemDefaultFocus();
			//	}
			//	ImGui::EndCombo();
			//}

			ImGui::DragFloat("FOV", &EditorCamera.FOV);
			ImGui::DragFloat("Pitch Rate", &EditorCamera.PitchRate);
			ImGui::DragFloat("Yaw rate", &EditorCamera.YawRate);
			ImGui::DragFloat("Speed", &EditorCamera.Speed);

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Rendering", tree_node_flags))
		{


			ImGui::TreePop();
		}

		ImGui::End();
	}
}

