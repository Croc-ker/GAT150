#pragma once
#include "Framework/Actor.h"


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
		void OnCollision(Actor* other);
	private:
		float speed = 0;
		float turnRate = 0;
	};
}