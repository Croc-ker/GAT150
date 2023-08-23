//#include "Bomb.h"
//#include "../../../../Audio/AudioSystem.h"
//
//void Bomb::Update(float dt)
//{
//	kiko::Actor::Update(dt);
//	if (tag == "BombActive") {
//		tag = "BombHit";
//		lifespan = 2.0f;
//		transform.scale *= 75.0f;
//		kiko::g_AudioSystem.PlayOneShot("death");
//	}
//}
//
//void Bomb::OnCollision(Actor* other)
//{
//	if (other->tag == "Player" && tag == "BombPickup") {
//		tag = "BombActive";
//	}
//}
