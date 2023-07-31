#pragma once
#include "Framework/Actor.h"
#include <Core/Random.h>
class WeaponType : public kiko::Actor
{
public:
	enum Type
	{
		NormalShot,
		DoubleShot,
		BigShot
	};
	WeaponType() = default;
	WeaponType(const kiko::Transform& transform, std::shared_ptr<kiko::Model> model) :
		Actor{ transform, model }
	{}
	void Update(float dt) override;
	void OnCollision(Actor* other) override;
	void GiveNormalShot(Actor* player);
	void GiveDoubleShot(Actor* player);
	void GiveBigShot(Actor* player);

	void SetRandomType();
private:
	Type m_type;
};