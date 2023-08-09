#pragma once
#include "Framework/Actor.h"
#include "Core/Math/Transform.h"

class Bomb : public kiko::Actor
{
public:
	Bomb() = default;
	Bomb(const kiko::Transform& transform) :
		Actor{ transform }
	{
		m_lifespan = 5.0f;
	}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;
private:

};

