#include "Player.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include"Renderer/Model.h"
#include "Weapon.h"
#include "Framework/Scene.h"
#include "../../Audio/AudioSystem.h"
#include "SpaceGame.h"
#include "Core/Math/MathUtils.h"
#include "Framework/Components/PhysicsComponent.h"
#include "Renderer/Texture.h"
#include "Framework/Components/SpriteComponent.h"
#include "Framework/Resource/ResourceManager.h"
#include <Framework/Components/PhysicsComponent.h>
#include <Framework/Components/EnginePhysicsComponent.h>


void Player::Update(float dt)
{
	Actor::Update(dt);

	float rotate = 0;
	if (kiko::g_InputSystem.GetKeyDown(SDL_SCANCODE_A)) rotate = -1;
	if (kiko::g_InputSystem.GetKeyDown(SDL_SCANCODE_D)) rotate = 1;
	m_transform.rotation += rotate * m_turnRate * kiko::g_time.GetDeltaTime();

	float thrust = 0;
	if (kiko::g_InputSystem.GetKeyDown(SDL_SCANCODE_W)) thrust = 1;

	kiko::vec2 forward = kiko::vec2{ 0,-1 }.Rotate(m_transform.rotation);
	Addforce(forward * m_speed * thrust);

	auto phyicsComponent = GetComponent<kiko::PhysicsComponent>();
	phyicsComponent->ApplyForce(forward * m_speed * thrust);

	//m_transform.position += forward * m_speed * thrust * kiko::g_time.GetDeltaTime();
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_Renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_Renderer.GetHeight());

	if (kiko::g_InputSystem.GetKeyDown(SDL_SCANCODE_SPACE) && !kiko::g_InputSystem.GetPreviousKeyDown(SDL_SCANCODE_SPACE)) {
		switch (m_type)
		{
		case WeaponType::Type::NormalShot:
			{
				kiko::Transform transform{ m_transform.position, m_transform.rotation + kiko::DegreesToRadians(0.0), 3.0 };
				std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(50.0f, transform);
				weapon->m_tag = "PlayerBullet";

				std::unique_ptr<kiko::SpriteComponent> component = std::make_unique<kiko::SpriteComponent>();
				component->m_texture = kiko::g_resources.Get<kiko::Texture>("pikmin.png", kiko::g_Renderer);
				this->AddComponent(std::move(component));

				m_scene->Add(std::move(weapon));
				kiko::g_AudioSystem.PlayOneShot("laser");
			}
				break;
		case WeaponType::Type::DoubleShot:
			{
				kiko::Transform transform{ m_transform.position, m_transform.rotation + kiko::DegreesToRadians(10.0f), 2.0f };
				std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(40.0f, transform);
				weapon->m_tag = "PlayerBullet";
				m_scene->Add(std::move(weapon));
				kiko::g_AudioSystem.PlayOneShot("laser");
				kiko::Transform transform2{ m_transform.position, m_transform.rotation - kiko::DegreesToRadians(10.0f), 1};
				weapon = std::make_unique<Weapon>(40.0f, transform2);
				weapon->m_tag = "PlayerBullet";
				weapon->SetLifespan(0.9f);
				m_scene->Add(std::move(weapon));
				kiko::g_AudioSystem.PlayOneShot("doubleshot");
			}
				break;
		case WeaponType::Type::BigShot:
			{
				kiko::Transform transform{ m_transform.position, m_transform.rotation + kiko::DegreesToRadians(0.0f), 10.0f };
				std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(15.0f, transform);
				weapon->m_tag = "PlayerBullet";
				weapon->SetLifespan(5.0f);
				m_scene->Add(std::move(weapon));
				kiko::g_AudioSystem.PlayOneShot("bigshot");
			}
				break;
			default:
			{
				kiko::Transform transform{ m_transform.position, m_transform.rotation + kiko::DegreesToRadians(0.0f), 3.0f };
				std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(50.0f, transform);
				weapon->m_tag = "PlayerBullet";
				m_scene->Add(std::move(weapon));
				kiko::g_AudioSystem.PlayOneShot("laser");
			}
				break;
		}
	}



	if (kiko::g_InputSystem.GetKeyDown(SDL_SCANCODE_T)) kiko::g_time.SetTimeScale(0.5f);
	else kiko::g_time.SetTimeScale(1.0f);

}
void Player::OnCollision(Actor* other)
{
	if (other->m_tag == "EnemyBullet") {
		m_health -= 1;
		if (m_health <= 0) {
			m_game->SetLives(m_game->GetLives() - 1);
			m_destroyed = true;
			kiko::g_AudioSystem.PlayOneShot("death");
			dynamic_cast<SpaceGame*>(m_game)->SetState(SpaceGame::eState::PlayerDeadStart);
		}
	}
}

void Player::SetWeaponType(WeaponType::Type type)
{
	m_type = type;
}
