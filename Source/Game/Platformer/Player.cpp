#include "Player.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include"Renderer/ModelManager.h"
#include "Audio/AudioSystem.h"
#include "Core/Math/MathUtils.h"
#include "Physics/PhysicsSystem.h"
#include "Core/Random.h"
#include "Platformer.h"
#include "Framework/Framework.h"

namespace kiko {
	CLASS_DEFINITION(Player)

		bool Player::Initialize()
	{
		Actor::Initialize();

		m_physicsComponent = GetComponent<PhysicsComponent>();
		m_spriteComponent = GetComponent<SpriteAnimRenderComponent>();

		return true;
	}

	void Player::Update(float dt)
	{
		Actor::Update(dt);
		vec2 velocity = m_physicsComponent->velocity;
		float dir = 0;
		if (g_InputSystem.GetKeyDown(SDL_SCANCODE_A)) 
		{
			dir = -1.0;
		}
		if (g_InputSystem.GetKeyDown(SDL_SCANCODE_D)) 
		{
			dir = 1.0;
		}
		//transform.rotation += rotate * m_turnRate * kiko::g_time.GetDeltaTime();
		
		if (dir) {
			velocity.x = dir * speed * ((grounded) ? 1 : 1.2) * dt;
			velocity.x = Clamp(velocity.x, -maxSpeed, maxSpeed);
			m_physicsComponent->SetVelocity(velocity);
			//m_physicsComponent->ApplyForce(forward * speed * dir * ((grounded) ? 1 : 0.25));
		}
		//jump
		kiko::vec2 up = kiko::vec2{ 0,-1 };
		if (grounded && kiko::g_InputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_InputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)) {
			m_physicsComponent->SetVelocity(velocity + (up * jump));
		}
		//attack
		if (kiko::g_InputSystem.GetKeyDown(SDL_SCANCODE_E) && !kiko::g_InputSystem.GetPreviousKeyDown(SDL_SCANCODE_E)) {
			
		}
		//m_physicsComponent->SetGravityScale((velocity.y > 0) ? 0.5 : 1);
		//animation
		if (std::fabs(velocity.x) > 0.2f)
		{
			if (dir != 0)m_spriteComponent->flipH = (dir < 0);
			m_spriteComponent->SetSequence("run");
		}
		else
		{
			m_spriteComponent->SetSequence("idle");
		}
	}
	bool Player::OnCollisionEnter(Actor* other)
	{
		bool touchCoin = false;
		name = other->name;
		if (other->tag == "Ground" || other->tag == "Water" || other->tag == "Flipper") {
			grounded = true;			
		}
		if (other->tag == "Flipper") {
			m_physicsComponent->SetGravityScale(m_physicsComponent->gravityScale * -1);

		}
		if (name == "Coin") {
			other->m_destroyed = true;
			touchCoin = true;
		}
		return touchCoin;
	}

	void Player::OnCollisionExit(Actor* other)
	{
		if (other->tag == "Ground" || other->tag == "Sticky") {
			grounded = false;
		}
	}

	void Player::Read(const json_t& value) {
		Actor::Read(value);

		READ_DATA(value, speed);
		READ_DATA(value, jump);
		READ_DATA(value, maxSpeed);
	}
}