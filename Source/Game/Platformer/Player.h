#pragma once
#include "Framework/Actor.h"


namespace kiko {
	class Player : public kiko::Actor
	{
	public:

		CLASS_DECLARATION(Player)

			bool Initialize() override;

		void Update(float dt) override;
		bool OnCollisionEnter(Actor* other) override;
		void OnCollisionExit(Actor* other) override;
		int m_health = 3;

	private:
		float speed = 0;
		float maxSpeed = 0;
		float jump = 0;
		bool grounded = false;


		class PhysicsComponent* m_physicsComponent = nullptr;
		class SpriteAnimRenderComponent* m_spriteComponent = nullptr;
	};
}