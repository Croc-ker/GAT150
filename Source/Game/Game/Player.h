#pragma once
#include "Framework/Actor.h"
#include "WeaponType.h"
enum Type
{
	NormalShot,
	DoubleShot,
	BigShot,
	SeekShot
};

class Player : public kiko::Actor
{

public:
	Player() = default;
	Player(float speed, float turnRate, const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		Actor{ transform, model },
		m_speed{ speed },
		m_turnRate{ turnRate }
	{}

	void Update(float dt) override;
	void OnCollision(Actor* other) override;
	void SetWeaponType(WeaponType::Type type);

	int m_health = 3;
private:
	float m_speed = 0;
	float m_turnRate = 0;
	WeaponType::Type m_type;
};