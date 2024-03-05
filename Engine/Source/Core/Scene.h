#pragma once
#include "entt.hpp"

#include "Core/Camera.h"
#include "Core/Entity.h"

#include "Rendering/Shader.h"// todo temp
#include "Rendering/FrameBuffer.h"// todo temp

namespace AN
{
	class FEntity;

	class FScene
	{
	public:
		FScene();

		void Init();
		void AddEntity();
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
		bool bDrawGrid = true;
		bool bVSync = false;

		FMaterial Mat;
		FMaterial GridMat;
		FVertexBuffer Buffer;
		FVertexBuffer GridBuffer;

		std::vector<FEntity> Entities;
		entt::registry EntityRegistry;
		mutable unsigned int SelectedEntityId = -1;
	};
}