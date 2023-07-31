#pragma once
#include "Framework/Actor.h"
#include "WeaponType.h"

class Weapon : public kiko::Actor
{
public:
	Weapon() = default;
	Weapon(float speed, const kiko::Transform& transform, std::shared_ptr<kiko::Model>  model) :
		Actor{ transform, model },
		m_speed{ speed }
	{
		if (m_type != WeaponType::Type::BigShot) {
			m_lifespan = 1.0f;
		}
		else {
			m_lifespan = 10.0f;
		}
	}

	void Update(float dt) override;
	void OnCollision(Actor* other) override;

private:
	float m_speed = 0;
	float m_turnRate = 0;
	WeaponType::Type m_type;
};