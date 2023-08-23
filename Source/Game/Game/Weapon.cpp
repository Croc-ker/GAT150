#include "Weapon.h"
#include "Renderer/Renderer.h"
#include "Framework/Components/CollisionComponent.h"
#include "Framework/Components/RenderComponent.h"


namespace kiko {

	CLASS_DEFINITION(Weapon)


		bool Weapon::Initialize()
	{
		Actor::Initialize();


		auto collisonComponent = GetComponent<kiko::CollisionComponent>();
		if (collisonComponent)
		{
			auto renderComponent = GetComponent<kiko::RenderComponent>();

			if (renderComponent) {
				float scale = transform.scale;
				collisonComponent->m_radius = renderComponent->GetRadius() * scale;
			}

		}




		return true;
	}

	void Weapon::Update(float dt)
	{

		Actor::Update(dt);

		kiko::vec2 forward = kiko::vec2{ 0,-1 }.Rotate(transform.rotation);
		transform.position += forward * speed * speed * kiko::g_time.GetDeltaTime();
		transform.position.x = kiko::Wrap(transform.position.x, (float)kiko::g_Renderer.GetWidth());
		transform.position.y = kiko::Wrap(transform.position.y, (float)kiko::g_Renderer.GetHeight());
	}

	void Weapon::OnCollision(Actor* other)
	{
		if (other->tag + "Bullet" != tag && other->tag != tag) {
			m_destroyed = true;
		}
	}

	void Weapon::Read(const json_t& value) {
		Actor::Read(value);

		READ_DATA(value, speed);
	}
}