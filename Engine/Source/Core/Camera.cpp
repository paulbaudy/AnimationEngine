
#include "Camera.h"

#include <algorithm>

namespace AN
{
	void FCamera::Update(const float& InDeltaTime)
	{
		upAngle = upAngle + (DeltaY * PitchRate);
		rightAngle = rightAngle + (DeltaX * YawRate);

		// todo investigate weird pitch values
		upAngle = std::clamp(upAngle, -90.f, 90.f);

		glm::quat q = glm::angleAxis(glm::radians(-rightAngle), glm::vec3(0, 1, 0));
		q *= glm::angleAxis(glm::radians(-upAngle), glm::vec3(1, 0, 0));

		Transform.SetQuat(q);

		glm::vec3 ForwardDir = glm::rotate(Transform.GetQuat(), glm::vec3(0.f, 0.f, 1.f));
		glm::vec3 RightDir = glm::cross(ForwardDir, glm::vec3(0.f, 1.f, 0.f));

		Transform.Translation -= Input.x * ForwardDir * Speed * InDeltaTime;
		Transform.Translation -= Input.y * RightDir * Speed * InDeltaTime;

		DeltaX = 0.f;
		DeltaY = 0.f;
	}
}