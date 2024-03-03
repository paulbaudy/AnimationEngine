#pragma once

#include "Core.h"
#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

#include "entt.hpp"

namespace AN
{


	class FCamera
	{

	};

	class IComponent
	{
	public:
		virtual void Update() {}
		virtual void Render() {}
	};

	class FSceneComponent : public IComponent
	{

	};

	class FTransformComponent : public IComponent
	{
	public:
		void SetEuler(const glm::vec3& InRot)
		{
			RotEuler = InRot;
			RotQuat = glm::quat(RotEuler);
		}

	public:
		glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };

	private:
		glm::vec3 RotEuler = { 0.f, 0.f, 0.f };
		glm::quat RotQuat = { 1.0f, 0.0f, 0.0f, 0.0f };
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
		}

		template<typename T>
		T& AddComponent();

		unsigned int GetID() const { return ID; }
	private:
		entt::entity Handle{ entt::null };
		unsigned int ID;
		std::string Name;
		FScene* Scene;
	};

	class FScene
	{
	public:

		void AddEntity();
		void Update();
		void Render();


		entt::registry& GetRegistry()
		{
			return EntityRegistry;
		}

		std::vector<FEntity> Entities;


	private:
		entt::registry EntityRegistry;
	};


	template<typename T>
	inline T& FEntity::AddComponent()
	{
		return Scene->GetRegistry().emplace<T>(Handle);
	}
}