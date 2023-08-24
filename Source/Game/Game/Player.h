#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"

namespace kiko
{

	class Player : public kiko::Actor
	{

	public:
		Player() = default;
		Player(float speed, float turnRate, const kiko::Transform& transform) :
			Actor{ transform },
			speed{ speed },
			turnRate{ turnRate }
		{}

		bool Initialize() override;
		void Update(float dt) override;
		void OnCollision(Actor* other) override;

		CLASS_DECLARATION(Player);

	private:
		float speed = 0;
		float turnRate = 0;
		int health = 100;

		kiko::PhysicsComponent* m_physicsComponent = nullptr;
	};
}