#pragma once
#include "Framework/Actor.h"
#include "Framework/Components/PhysicsComponent.h"

namespace kiko
{
	class Enemy : public kiko::Actor
	{
	public:
		CLASS_DECLARATION(Enemy);

		Enemy() = default;
		Enemy(float speed, float turnRate, const kiko::Transform& transform) :
			Actor{ transform },
			speed{ speed },
			turnRate{ turnRate }
		{
			m_fireTime = 3.0f;
			m_fireTimer = m_fireTime;
		}
		bool Initialize() override;

		void Update(float dt) override;
		void OnCollisionEnter(Actor* other) override;

		bool GetFacingPlayer() const { return m_facingPlayer; }
		void SetFacingPlayer(bool facingPlayer) { m_facingPlayer = facingPlayer; }
	public:
		float speed = 0;
		float turnRate = 0;

		bool m_facingPlayer = false;

		float m_fireTime = 0;
		float m_fireTimer = 0;

		kiko::PhysicsComponent* m_physicsComponent = nullptr;

		int m_health = 50;
	};
}
