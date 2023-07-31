#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"
#include "WeaponType.h"
#include "Framework/Scene.h"
#include "../../Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/Renderer.h"
#include "Renderer/Text.h"
#include "Renderer/ModelManager.h"
#include "Framework/Emitter.h"
#include "Renderer/ParticleSystem.h"
#include "Core/Random.h"
#include "Bomb.h"

bool SpaceGame::Initialize()
{
	// create font / text objects
	m_font = std::make_shared<kiko::Font>("Impact Label.ttf", 24);

	m_scoreText = std::make_unique<kiko::Text>(m_font);
	m_scoreText->Create(kiko::g_Renderer, "SCORE: 0", kiko::Color{1, 1, 1, 1});

	m_titleText = std::make_unique<kiko::Text>(m_font);
	m_titleText->Create(kiko::g_Renderer, "SPACE", kiko::Color{1, 1, 1, 1});

	m_gameOverText = std::make_unique<kiko::Text>(m_font);
	m_gameOverText->Create(kiko::g_Renderer, "GAME OVER", kiko::Color{1, 1, 1, 1});
	
	m_livesText = std::make_unique<kiko::Text>(m_font);
	m_livesText->Create(kiko::g_Renderer, "LIVES: 0", kiko::Color{1, 1, 1, 1});

	//load audio
	kiko::g_AudioSystem.AddAudio("laser", "laser.wav");
	kiko::g_AudioSystem.AddAudio("bigshot", "bigshot.wav");
	kiko::g_AudioSystem.AddAudio("doubleshot", "doubleshot.wav");
	kiko::g_AudioSystem.AddAudio("explode", "explode.wav");
	kiko::g_AudioSystem.AddAudio("pickup", "pickup.wav");
	kiko::g_AudioSystem.AddAudio("death", "death.wav");
	kiko::g_AudioSystem.AddAudio("song", "song.wav");

	kiko::g_AudioSystem.PlayOneShot("song", true);

	m_scene = std::make_unique<kiko::Scene>();

	return true;
}

void SpaceGame::Shutdown()
{
}

void SpaceGame::Update(float dt)
{
	switch (m_state)
	{
	case SpaceGame::eState::Title:
		if (kiko::g_InputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			m_state = eState::StartGame;
		}
		break;
	case SpaceGame::eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_state = eState::StartLevel;
		break;
	case SpaceGame::eState::StartLevel:
		m_scene->RemoveAll();
		{
			std::unique_ptr<Player> player = std::make_unique<Player>(5.0f, kiko::Pi, kiko::Transform{ {400, 300}, 0, 6 }, kiko::g_manager.Get("ship.txt"));
			player->m_tag = "Player";
			player->m_game = this;
			player->SetDamping(0.9f);
			player->SetWeaponType(WeaponType::Type::NormalShot);
			m_scene->Add(move(player));
		}
		m_state = eState::Game;
		break;
	case SpaceGame::eState::Game:
		m_spawnTimer += dt;
		if (m_spawnTimer >= m_spawnTime) {
			m_spawnTimer = 0;
			std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(10.0f, kiko::Pi, kiko::Transform{ {kiko::random(600), kiko::random(600)}, kiko::randomf(kiko::TwoPi), 8.0f }, kiko::g_manager.Get("enemy.txt"));
			enemy->m_tag = "Enemy";
			enemy->m_game = this;
			m_scene->Add(move(enemy));
			int diceRoll = kiko::random(20);
			std::cout << diceRoll << "\n";
			if (diceRoll <= 15) {
				std::unique_ptr<WeaponType> weaponPickup = std::make_unique<WeaponType>(kiko::Transform{ {kiko::random(600), kiko::random(600)}, kiko::randomf(kiko::TwoPi), 2.0f }, kiko::g_manager.Get("pickup.txt"));
				weaponPickup->m_game = this;
				weaponPickup->SetRandomType();
				weaponPickup->SetLifespan(5.0f);
				m_scene->Add(move(weaponPickup));
			}
			else {
				std::unique_ptr<Bomb> bombPickup = std::make_unique<Bomb>(kiko::Transform{ {kiko::random(100,701), kiko::random(100,501)}, kiko::randomf(kiko::TwoPi), 1.0f }, kiko::g_manager.Get("bomb.txt"));
				bombPickup->m_game = this;
				bombPickup->m_tag = "BombPickup";
				bombPickup->SetLifespan(5.0f);
				m_scene->Add(move(bombPickup));
			}
		}

		break;
	case SpaceGame::eState::PlayerDeadStart:
		m_stateTimer = 200;

		if (m_lives == 0) {
			m_state = eState::GameOver;
		}
		else m_state = eState::PlayerDead;
		break;
	case SpaceGame::eState::PlayerDead:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0)
		{
			
			m_state = eState::StartLevel;
		}
		break;
	case SpaceGame::eState::GameOver:
		m_stateTimer -= dt;
		if (m_stateTimer == 0)
		{
			m_state = eState::Title;
		}
		break;
	default:
		break;
	}
	m_scoreText->Create(kiko::g_Renderer, "SCORE: " + std::to_string(m_score), { 1,1,1,1 });
	m_livesText->Create(kiko::g_Renderer, "LIVES: " + std::to_string(m_lives), { 1,1,1,1 });
	m_scene->Update(dt);
}
void SpaceGame::Draw(kiko::Renderer& renderer)
{
	if (m_state == eState::Title) {
		m_titleText->Draw(renderer, 350, 300);
	}
	if (m_state == eState::GameOver) {
		m_gameOverText->Draw(renderer, 350, 300);
	}
	m_scoreText->Draw(renderer, 40, 40);
	m_livesText->Draw(renderer, 350, 40);
	kiko::g_particleSystem.Draw(renderer);
	m_scene->Draw(renderer);
}
