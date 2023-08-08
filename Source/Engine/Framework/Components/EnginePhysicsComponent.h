#pragma once
#include "PhysicsComponent.h"

namespace kiko
{
	class EnginePhysicsComponent : public PhysicsComponent
	{
		void Update(float dt) override;
		void ApplyForce(const vec2& force) override;
	};
}