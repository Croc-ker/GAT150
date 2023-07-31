#include "Bomb.h"
#include "../../Audio/AudioSystem.h"

void Bomb::Update(float dt)
{
	kiko::Actor::Update(dt);
	if (m_tag == "BombActive") {
		m_tag = "BombHit";
		m_lifespan = 2.0f;
		m_transform.scale *= 75.0f;
		kiko::g_AudioSystem.PlayOneShot("death");
	}
}

void Bomb::OnCollision(Actor* other)
{
	if (other->m_tag == "Player" && m_tag == "BombPickup") {
		m_tag = "BombActive";
	}
}
