#pragma once

#include "Core.h"

#include "Rendering/FrameBuffer.h"
#include "Rendering/Shader.h"
#include "Core/Components.h"

#include <vector>
#include <string>
#include <entt.hpp>


namespace AN
{
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

		template<typename T>
		T* GetComponent();

		unsigned int GetID() const { return ID; }
		entt::entity GetHandle() const { return Handle; }
		const std::string& GetLabel() const { return Name; }
		void SetLabel(const std::string& InLabel) { Name = InLabel; }
		std::string& GetLabel() { return Name; }
		const FScene* GetScene() const { return Scene; }
		FScene* GetScene() { return Scene; }

	private:
		entt::entity Handle{ entt::null };
		unsigned int ID;
		std::string Name;
		FScene* Scene;
	};

	template<typename T>
	inline T& FEntity::AddComponent()
	{
		return Scene->GetRegistry().emplace<T>(Handle);
	}

	template<typename T>
	inline T* FEntity::GetComponent()
	{
		return Scene->GetRegistry().try_get<T>(Handle);
	}
}