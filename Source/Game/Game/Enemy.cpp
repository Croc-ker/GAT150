#include "Enemy.h"
#include "Renderer/Renderer.h"
#include "Framework/Scene.h"
#include "Renderer/ModelManager.h"
#include "Player.h"
#include "Weapon.h"
#include "../../Audio/AudioSystem.h"
#include "SpaceGame.h"
#include <Framework/Emitter.h>



void Enemy::Update(float dt)
{
	Actor::Update(dt);

	SetFaceingPlayer(false);

	kiko::vec2 forward = kiko::vec2{ 0,-1 }.Rotate(m_transform.rotation);
	Player* player = m_scene->GetActor<Player>();
	if (player) {
		kiko::Vector2 direction = player->m_transform.position - m_transform.position;
		//turn torwards player
		float turnAngle = kiko::vec2::SignedAngle(forward, direction.Normalized());
		m_transform.rotation += turnAngle * dt;
		//check if player is in front
		if (std::fabs(turnAngle) < kiko::DegreesToRadians(30.0f))
		{
			SetFaceingPlayer(true);
		}
	}
	m_transform.position += forward * m_speed * m_speed * kiko::g_time.GetDeltaTime();
	m_transform.position.x = kiko::Wrap(m_transform.position.x, (float)kiko::g_Renderer.GetWidth());
	m_transform.position.y = kiko::Wrap(m_transform.position.y, (float)kiko::g_Renderer.GetHeight());
	m_fireTimer -= kiko::g_time.GetDeltaTime();
	if (m_fireTimer <= 0 && GetFaceingPlayer()) {
		kiko::Transform transform{ m_transform.position, m_transform.rotation, 1};
		std::unique_ptr<Weapon> weapon = std::make_unique<Weapon>(40.0f, transform, kiko::g_manager.Get("weapon.txt"));
		weapon->m_tag = "EnemyBullet";
		m_scene->Add(std::move(weapon));
		kiko::g_AudioSystem.PlayOneShot("laser");

		m_fireTimer = m_fireTime;
	}

}

void Enemy::OnCollision(Actor* other)
{
	if (other->m_tag == "PlayerBullet") {
		m_health -= 25;
		if (m_health <= 0) {
			m_destroyed = true;
			m_game->AddPoints(100);
			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 20;
			data.spawnRate = 10;
			data.angle = 0;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 0.5f;
			data.lifetimeMax = 1.5f;
			data.speedMin = 50;
			data.speedMax = 250;
			data.damping = 0.5f;
			data.color = kiko::Color{ 1, 1, 0, 1 };
			kiko::Transform transform{ { m_transform.position.x, m_transform.position.y }, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->SetLifespan(1.0f);
			m_scene->Add(std::move(emitter));
			kiko::g_AudioSystem.PlayOneShot("explode");
		}
	}
	else if (other->m_tag == "BombActive" || other->m_tag == "BombHit") {
		m_health -= 50;
		if (m_health <= 0) {
			m_destroyed = true;
			m_game->AddPoints(100);
			kiko::EmitterData data;
			data.burst = true;
			data.burstCount = 20;
			data.spawnRate = 10;
			data.angle = 0;
			data.angleRange = kiko::Pi;
			data.lifetimeMin = 0.5f;
			data.lifetimeMax = 1.5f;
			data.speedMin = 50;
			data.speedMax = 250;
			data.damping = 0.5f;
			data.color = kiko::Color{ 1, 1, 0, 1 };
			kiko::Transform transform{ { m_transform.position.x, m_transform.position.y }, 0, 1 };
			auto emitter = std::make_unique<kiko::Emitter>(transform, data);
			emitter->SetLifespan(1.0f);
			m_scene->Add(std::move(emitter));
			kiko::g_AudioSystem.PlayOneShot("explode");
		}
	}
	
}
