#pragma once

#include "Core.h"
#include <vector>
#include <string>
#include "entt.hpp"

#include "Rendering/FrameBuffer.h"
#include "Rendering/Shader.h"
#include "Core/Components.h"

namespace AN
{
	class FCamera
	{
	public:
		float FOV = 70.f;
		float PitchRate = 0.1f;
		float YawRate = 0.1f;
		float Speed = 0.1f;
		FTransformComponent Transform;
	};


	class FScene;

	class FEntity
	{
	public:
		FEntity(entt::entity InHandle, FScene* InOwner)
			: Handle(InHandle)
			, Scene(InOwner)
		{
			static unsigned int NumEntities = 0;
			ID = NumEntities++;
			Name = "Entity " + std::to_string(ID);
		}

		template<typename T>
		T& AddComponent();

		unsigned int GetID() const { return ID; }
		entt::entity GetHandle() const { return Handle; }
		const std::string& GetLabel() const { return Name; }
		std::string& GetLabel() { return Name; }
		const FScene* GetScene() const { return Scene; }
		FScene* GetScene() { return Scene; }

	private:
		entt::entity Handle{ entt::null };
		unsigned int ID;
		std::string Name;
		FScene* Scene;
	};

	class FScene
	{
	public:
		FScene();

		void Init();
		void AddEntity();
		void Update();
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

		FEntity* GetSelectedEntity()
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

		FMaterial Mat;
		FMaterial GridMat;
		FVertexBuffer Buffer;
		FVertexBuffer GridBuffer;

		std::vector<FEntity> Entities;
		entt::registry EntityRegistry;
		mutable unsigned int SelectedEntityId = -1;
	};


	template<typename T>
	inline T& FEntity::AddComponent()
	{
		return Scene->GetRegistry().emplace<T>(Handle);
	}
}