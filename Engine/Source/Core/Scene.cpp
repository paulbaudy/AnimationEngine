
#include "anpch.h"
#include "Scene.h"

#include "Core/Components.h"
#include "Core/Entity.h"
#include "UI/ComponentDescriptors.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/Shader.h"
#include "Window/Window.h"

#include <imgui.h>
#include <glad/glad.h>
#include <glm/gtx/quaternion.hpp>
#include <glm/trigonometric.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <GLFW/glfw3.h>


namespace AN
{
	FScene::FScene()
		: FrameBuffer(100.f, 100.f)
		, DepthBuffer(100.f, 100.f)
	{

	}

	void FScene::Init()
	{
		// Test Entity
		FEntity& TriangleEntity = AddEntity("Triangle");
		FMeshComponent& MeshComp = TriangleEntity.AddComponent<FMeshComponent>();

		MeshComp.Mesh.Geometry.Vertices = FPrimitives::GetTriangle();
		MeshComp.Mesh.Material.PushShader("Content/Shaders/Unlit.vert", GL_VERTEX_SHADER);
		MeshComp.Mesh.Material.PushShader("Content/Shaders/Unlit.frag", GL_FRAGMENT_SHADER);

		// Plane
		FEntity& Plane = AddEntity("Plane");
		FMeshComponent& PlaneMesh = Plane.AddComponent<FMeshComponent>();

		PlaneMesh.Mesh.Geometry.Vertices = FPrimitives::GetPlane();
		PlaneMesh.Mesh.Material.PushShader("Content/Shaders/Unlit.vert", GL_VERTEX_SHADER);
		PlaneMesh.Mesh.Material.PushShader("Content/Shaders/Unlit.frag", GL_FRAGMENT_SHADER);

		QuadBuffer.GenBuffers();
		QuadBuffer.Vertices = FPrimitives::GetQuad();

		// Grid Mesh
		GridMesh.Geometry.Vertices = FPrimitives::GetQuad();
		GridMesh.Material.PushShader("Content/Shaders/Grid.vert", GL_VERTEX_SHADER);
		GridMesh.Material.PushShader("Content/Shaders/Grid.frag", GL_FRAGMENT_SHADER);

		// Skybox
		Cubemap.GenBuffer();
		std::vector<std::string> faces
		{
			"Content/Textures/Skybox/miramar_ft.tga",
			"Content/Textures/Skybox/miramar_bk.tga",
			"Content/Textures/Skybox/miramar_up.tga",
			"Content/Textures/Skybox/miramar_dn.tga",
			"Content/Textures/Skybox/miramar_rt.tga",
			"Content/Textures/Skybox/miramar_lf.tga",
		};
		Cubemap.LoadTextures(faces);
		CubemapMat.PushShader("Content/Shaders/Skybox.vert", GL_VERTEX_SHADER);
		CubemapMat.PushShader("Content/Shaders/Skybox.frag", GL_FRAGMENT_SHADER);
		CubemapVert.GenBuffers();
		CubemapVert.Vertices = FPrimitives::GetSkybox();

		// Depth buffer material
		DepthMat.PushShader("Content/Shaders/Quad.vert", GL_VERTEX_SHADER);
		DepthMat.PushShader("Content/Shaders/Depth.frag", GL_FRAGMENT_SHADER);

		// 
		EditorCamera.Transform.Translation.z = 5.f;
		EditorCamera.Transform.Translation.y = 2.f;

		glGenTextures(1, &DepthTexture);
	}

	FEntity& FScene::AddEntity(const std::string& InName)
	{
		FEntity Entity(EntityRegistry.create(), this);
		if(InName.size() > 0)
			Entity.SetLabel(InName);

		// Add transform component by default. All entities are scene components by design
		Entity.AddComponent<FTransformComponent>();

		Entities.push_back(Entity);
		return Entities.back();
	}

	void FScene::Update(const float& InDeltaTime)
	{
		if (bCapturing)
		{
			EditorCamera.Update(InDeltaTime);
		}
	}

	void FScene::Render()
	{
	}

	void FScene::OnMouseMove(double xpos, double ypos)
	{
		if (!bCapturing)
		{
			EditorCamera.DeltaX = 0.f;
			EditorCamera.DeltaY = 0.f;

			MouseX = xpos;
			MouseY = ypos;
			return;
		}

		EditorCamera.DeltaX = (xpos - MouseX);
		EditorCamera.DeltaY = -(ypos - MouseY);

		MouseX = xpos;
		MouseY = ypos;
	}

	void FScene::OnKey(int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_W)
		{
			if(action == GLFW_PRESS)
				EditorCamera.Input.x += 1.f;
			else if (action == GLFW_RELEASE)
				EditorCamera.Input.x -= 1.f;

		}
		else if (key == GLFW_KEY_S)
		{
			if (action == GLFW_PRESS)
				EditorCamera.Input.x -= 1.f;
			else if (action == GLFW_RELEASE)
				EditorCamera.Input.x += 1.f;

		}
		else if (key == GLFW_KEY_D)
		{
			if (action == GLFW_PRESS)
				EditorCamera.Input.y += 1.f;
			else if (action == GLFW_RELEASE)
				EditorCamera.Input.y -= 1.f;

		}
		else if (key == GLFW_KEY_A)
		{
			if (action == GLFW_PRESS)
				EditorCamera.Input.y -= 1.f;
			else if (action == GLFW_RELEASE)
				EditorCamera.Input.y += 1.f;

		}
	}

	FEntity* FScene::GetSelectedEntity()
	{
		if (SelectedEntityId == -1)
			return nullptr;

		for (auto& Entity : Entities)
		{
			if (Entity.GetID() == SelectedEntityId)
			{
				return &Entity;
			}
		}
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

		FrameBuffer.Bind();
		auto size = ImGui::GetContentRegionAvail();
		// @todo move these on resize
		FrameBuffer.RescaleFrameBuffer(size.x, size.y);
		glViewport(0, 0, size.x, size.y);

		// Draw entities		
		static float NearClipPlane = 0.01f;
		static float FarClipPlane = 100.f;
		float Aspect = size.x / size.y;

		FRenderContext RenderContext;
		RenderContext.Projection = glm::perspective(glm::radians(EditorCamera.FOV), Aspect, NearClipPlane, FarClipPlane);
		RenderContext.View = glm::inverse(EditorCamera.Transform.ToMat());

		// Skybox
		if (bDrawSkybox)
		{
			// Reset view translation
			FRenderContext SkyBoxContext = RenderContext;
			FTransformComponent SkyboxTransform = EditorCamera.Transform;
			SkyBoxContext.View = glm::inverse(glm::mat4(glm::mat3(EditorCamera.Transform.ToMat())));

			glDepthMask(GL_FALSE);
			CubemapVert.BindBuffers();
			CubemapMat.Bind();
			glActiveTexture(GL_TEXTURE0);
			Cubemap.Bind();

			CubemapMat.SetInt("cubemap", 0);
			CubemapMat.SetMat4("projection", &RenderContext.Projection[0][0]);
			CubemapMat.SetMat4("view", &SkyBoxContext.View[0][0]);

			CubemapVert.Draw();

			Cubemap.Unbind();
			CubemapVert.UnbindBuffers();

			glDepthMask(GL_TRUE);
		}
	
		if (bDrawEntities)
		{
			auto ComponentsView = EntityRegistry.view<FTransformComponent, FMeshComponent>();
			for (const auto& Component : ComponentsView)
			{
				auto [Transform, MeshComp] = ComponentsView.get(Component);
				if (!MeshComp.bDraw)
					continue;

				RenderContext.Model = Transform.ToMat();
				MeshComp.Mesh.Draw(RenderContext);
			}
		}

		// Draw editor grid
		if (bDrawGrid)
		{
			GridMesh.Draw(RenderContext);
		}

		//DepthMat.Bind();
		//QuadBuffer.BindBuffers();
		//QuadBuffer.Draw();
		//QuadBuffer.UnbindBuffers();
		//DepthMat.Unbind();

		//ImGui::Image(
		//	(ImTextureID)DepthTexture,
		//	ImGui::GetContentRegionAvail(),
		//	ImVec2(0, 1),
		//	ImVec2(1, 0));

		//FrameBuffer.Unbind();

		ImGui::Image(
			(ImTextureID)FrameBuffer.getFrameTexture(),
			ImGui::GetContentRegionAvail(),
			ImVec2(0, 1),
			ImVec2(1, 0));


		if(false)
		{
			//glBindTexture(GL_TEXTURE_2D, 0);
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, DepthTexture);
			//
			//glBindFramebuffer(GL_FRAMEBUFFER, FrameBuffer.fbo);
			////glReadBuffer(GL_BACK); // Ensure we are reading from the back buffer.
			//glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size.x, size.y, size.x, size.y, 0);
			//glGenerateMipmap(GL_TEXTURE_2D);

			// GL_RGBA
			// 
			//glDepthMask(GL_FALSE);
			//FrameBuffer.Unbind();

			//DepthBuffer.Bind();
			//DepthBuffer.RescaleFrameBuffer(size.x, size.y);

			QuadBuffer.BindBuffers();
			DepthMat.Bind();

			glBindTexture(GL_TEXTURE_2D, FrameBuffer.DepthTexture);
			glActiveTexture(GL_TEXTURE0);
			DepthMat.SetInt("colorImage", 0);

			QuadBuffer.Draw();

			DepthMat.Unbind();
			QuadBuffer.UnbindBuffers();

			//DepthBuffer.Unbind();

			//glDepthMask(GL_TRUE);


			//ImGui::Image(
			//	(ImTextureID)FrameBuffer.getFrameTexture(),
			//	ImGui::GetContentRegionAvail(),
			//	ImVec2(0, 1),
			//	ImVec2(1, 0));
		}

		//ImGui::Image(
		//	(ImTextureID)FrameBuffer.DepthTexture,
		//	ImGui::GetContentRegionAvail(),
		//	ImVec2(0, 1),
		//	ImVec2(1, 0));

		FrameBuffer.Unbind();

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			LOG_ERROR("{0}", err);
			// Process/log the error.
		}


		ImGui::End();
	}


	void FScene::DrawSceneSettings()
	{
		ImGui::Begin("Scene");

		ImGuiTreeNodeFlags tree_node_flags = ImGuiTreeNodeFlags_SpanAllColumns | ImGuiTreeNodeFlags_DefaultOpen;

		if (ImGui::TreeNodeEx("Camera", tree_node_flags))
		{
			ImGui::Image(
				(ImTextureID)DepthBuffer.DepthTexture,
				ImGui::GetContentRegionAvail(),
				ImVec2(0, 1),
				ImVec2(1, 0));
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
			ImGui::DragFloat3("Translation", &EditorCamera.Transform.Translation.x);
			ImGui::DragFloat3("Rotation", &EditorCamera.Transform.RotEuler.x);
			ImGui::DragFloat3("Scale", &EditorCamera.Transform.Scale.x);

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Rendering", tree_node_flags))
		{
			if (ImGui::Checkbox("VSync", &bVSync))
			{
				glfwSwapInterval(bVSync ? 1 : 0);
			}

			ImGui::Checkbox("Draw Depth Buffer", &bDrawDepth);

			ImGui::TreePop();
		}

		if (ImGui::TreeNodeEx("Debug", tree_node_flags))
		{
			ImGui::Checkbox("Draw Grid", &bDrawGrid);
			ImGui::Checkbox("Draw Skybox", &bDrawSkybox);
			ImGui::Checkbox("Draw Entities", &bDrawEntities);

			ImGui::TreePop();
		}

		ImGui::End();
	}
}