#pragma once
#include "entt.hpp"

#include "Core/Camera.h"
#include "Core/Entity.h"

#include "Rendering/Shader.h"// todo temp
#include "Rendering/FrameBuffer.h"// todo temp
#include "Rendering/Cubemap.h"// todo temp

namespace AN
{
	class FEntity;

	class FScene
	{
	public:
		FScene();

		void Init();
		FEntity& AddEntity(const std::string& InName = std::string());
		void Update(const float& InDeltaTime);
		void Render();

		void OnMouseMove(double xpos, double ypos);
		void OnKey(int key, int scancode, int action, int mods);

		entt::registry& GetRegistry()
		{
			return EntityRegistry;
		}

		const entt::registry& GetRegistry() const
		{
			return EntityRegistry;
		}

		FEntity* GetSelectedEntity();

	public:
		// -- ImGui editor interface --
		void DrawEntities();
		void DrawEntity(FEntity& InEntity);
		void DrawViewport();
		void DrawSceneSettings();

	private:
		FCamera EditorCamera;
		double MouseX = 0.f;
		double MouseY = 0.f;
		bool bCapturing = false;
		bool bDrawGrid = false;
		bool bDrawEntities = true;
		bool bDrawSkybox = true;
		bool bVSync = false;
		bool bDrawDepth = false;

		FMesh GridMesh;
		FCubeMapTexture Cubemap;
		FMaterial CubemapMat;
		FVertexBuffer CubemapVert;

		FFrameBuffer FrameBuffer;
		// Depth buffer visualizer
		FFrameBuffer DepthBuffer;
		FVertexBuffer QuadBuffer;

		unsigned int DepthTexture = -1;

		FMaterial DepthMat;

		std::vector<FEntity> Entities;
		entt::registry EntityRegistry;
		mutable unsigned int SelectedEntityId = -1;
	};
}