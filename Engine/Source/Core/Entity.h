#pragma once

#include "Core.h"
#include <vector>
#include <string>
#include "entt.hpp"

#include "Rendering/FrameBuffer.h"
#include "Rendering/Shader.h"

namespace AN
{
	class FCamera
	{
	public:
		float FOV = 90.f;
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

	private:
		FCamera EditorCamera;
		FMaterial Mat;
		FVertexBuffer Buffer;

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