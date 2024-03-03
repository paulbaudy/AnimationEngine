#include "Entity.h"

namespace AN
{
	void FScene::AddEntity()
	{
		FEntity NewEntity(EntityRegistry.create(), this);

		// Add transform component by default. All entities are scene components by design
		NewEntity.AddComponent<FTransformComponent>();

		Entities.push_back(NewEntity);
	}

	void FScene::Update()
	{
	}


	void FScene::Render()
	{
	}

}

