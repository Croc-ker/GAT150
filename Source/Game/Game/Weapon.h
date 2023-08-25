#pragma once
#include "Framework/Actor.h"
#include <Framework/Components/PhysicsComponent.h>


namespace kiko {
	class Weapon : public kiko::Actor
	{
	public:

		CLASS_DECLARATION(Weapon)

			/*
			Weapon() = default;
			Weapon(float speed, const kiko::Transform& transform) :
				Actor{ transform },
				speed{ speed }
			{
				lifespan = 2.0f;
			}
			*/

		bool Initialize() override;
		void Update(float dt) override;
		virtual void OnCollisionEnter(Actor* other) {}
		virtual void OnCollisionExit(Actor* other) {}
	private:
		float speed = 0;
		float turnRate = 0;
		PhysicsComponent* m_physicsComponent;
	};
}