#pragma once
#include "Framework/Actor.h"
#include <Core/Transform.h>

class Bomb : public kiko::Actor
{
public:
	Bomb() = default;
	Bomb(const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		Actor{ transform, model }
	{
		m_lifespan = 5.0f;
	}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;
private:

};

