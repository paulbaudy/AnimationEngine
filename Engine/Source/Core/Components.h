#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

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
		RotQuat = InRot;
		// todo update eular
	}

public:
	glm::vec3 Translation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 Scale = { 1.0f, 1.0f, 1.0f };
	glm::vec3 RotEuler = { 0.f, 0.f, 0.f };

private:
	glm::quat RotQuat = { 1.0f, 0.0f, 0.0f, 0.0f };
};
