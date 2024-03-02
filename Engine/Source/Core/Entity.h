#pragma once

#include "Core.h"
#include <vector>

namespace AN
{
	class IComponent
	{
	public:
		virtual void Update() {}
		virtual void Render() {}
	};

	class FSceneComponent : public IComponent
	{

	};

	class FEntity
	{
	public:

		FSceneComponent Root;
		std::vector<TUniquePtr<IComponent>> Components;
	};

	class FScene
	{
		
		std::vector<TUniquePtr<FEntity>> Entities;
	};
}