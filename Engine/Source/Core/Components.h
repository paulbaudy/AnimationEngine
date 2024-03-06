#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Rendering/Mesh.h"

class IComponent
{
public:
	virtual void Update() {}
	virtual void Render() {}
};

class FSceneComponent : public IComponent
{

};

class FMeshComponent : public IComponent
{
public:
	AN::FMesh Mesh;
	bool bDraw = true;
};

class FTransformComponent : public IComponent
{
public:
	void SetEuler(const glm::vec3& InRot)
	{
		RotEuler = InRot;
		RotQuat = glm::quat(RotEuler);
	}

	glm::mat4 ToMat() const
	{
		return glm::translate(glm::mat4(1.0f), Translation)
			* glm::toMat4(RotQuat)
			* glm::scale(glm::mat4(1.0f), Scale);
	}

	glm::quat GetQuat() const
	{
		return RotQuat;
	}

	void SetQuat(const glm::quat& InRot)
	{
		const glm::vec3 OldEuler = RotEuler;

		RotQuat = InRot;
		RotEuler = glm::eulerAngles(InRot);


		if (
			(fabs(RotEuler.x - OldEuler.x) == glm::pi<float>()) &&
			(fabs(RotEuler.z - OldEuler.z) == glm::pi<float>())
			)
		{
			RotEuler.x = OldEuler.x;
			RotEuler.y = glm::pi<float>() - RotEuler.y;
			RotEuler.z = OldEuler.z;
		}
	}

	glm::vec3 Forward() const
	{
		return glm::rotate(GetQuat(), glm::vec3(0.f, 0.f, 1.f));
	}

	glm::vec3 Up() const
	{
		return glm::rotate(GetQuat(), glm::vec3(0.0f, 1.0f, 0.0f));
	}

public:
	glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3 RotEuler = { 0.f, 0.f, 0.f };

private:
	glm::quat RotQuat = { 1.0f, 0.0f, 0.0f, 0.0f };
};
