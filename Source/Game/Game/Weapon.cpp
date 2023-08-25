#include "Weapon.h"
#include "Renderer/Renderer.h"
#include "Framework/Components/CollisionComponent.h"
#include "Framework/Components/RenderComponent.h"


namespace kiko {

	CLASS_DEFINITION(Weapon)


		bool Weapon::Initialize()
	{
		Actor::Initialize();
		m_physicsComponent = GetComponent<PhysicsComponent>();

		auto collisonComponent = GetComponent<kiko::CollisionComponent>();
		if (collisonComponent)
		{

		}

		return true;
	}

	void Weapon::Update(float dt)
	{

		Actor::Update(dt);

		kiko::vec2 forward = kiko::vec2{ 0,-1 }.Rotate(transform.rotation);
		m_physicsComponent->SetVelocity(forward * speed);

		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_Renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_Renderer.GetHeight());
	}

	//void Weapon::OnCollisionEnter(Actor* other)
	//{
	//	//if (other->tag + "Bullet" != tag && other->tag != tag) {
	//	//	destroyed = true;
	//	//}
	//}
	//void Weapon::OnCollisionExit(Actor* other)
	//{
	//	//if (other->tag + "Bullet" != tag && other->tag != tag) {
	//	//	destroyed = true;
	//	//}
	//}

	void Weapon::Read(const json_t& value) {
		Actor::Read(value);

		READ_DATA(value, speed);
	}
}