#pragma once

#include "Components.h"

namespace AN
{
	class FCamera
	{
	public:
		void Update(const float& InDeltaTime);

		float FOV = 70.f;
		float PitchRate = 0.1f;
		float YawRate = 0.1f;
		float Speed = 5.f;
		FTransformComponent Transform;

		// Input vector
		// todo vec2 int instead of doubles
		double DeltaX = 0.f;
		double DeltaY = 0.f;

		float upAngle = 0.f;
		float rightAngle = 0.f;
		glm::vec2 Input;
	};
}