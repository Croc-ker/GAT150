#include "EnginePhysicsComponent.h"
#include "Framework/Actor.h"
namespace kiko {

	CLASS_DEFINITION(EnginePhysicsComponent);

	void kiko::EnginePhysicsComponent::Update(float dt)
	{
		m_owner->transform.position += velocity * dt;
		velocity *= std::pow(1.0f - damping, dt);
	}

	void kiko::EnginePhysicsComponent::ApplyForce(const vec2& force)
	{
		velocity += force;
	}
	void kiko::EnginePhysicsComponent::ApplyTorque(float torque)
	{

	}

	void EnginePhysicsComponent::Read(const json_t& value) {}
}