#include "SpaceGame.h"
#include "Player.h"
#include "Enemy.h"

#include "Framework/Framework.h"
#include "Renderer/Renderer.h"

#include "../../Audio/AudioSystem.h"
#include "Input/InputSystem.h"


#include "Renderer/ModelManager.h"


bool SpaceGame::Initialize()
{
	// create font / text objects
	m_font = GET_RESOURCE(kiko::Font, "Impact Label.ttf", 24);//std::make_shared<kiko::Font>("BlackHanSans-Regular.ttf", 24);
	m_scoreText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Impact Label.ttf", 24));
	m_scoreText->Create(kiko::g_Renderer, "SCORE: 000", kiko::Color{1, 1, 1, 1});

	m_livesText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Impact Label.ttf", 24));
	m_livesText->Create(kiko::g_Renderer, "LIVES: 0", kiko::Color{1, 1, 1, 1});

	m_titleText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Impact Label.ttf", 24));
	m_titleText->Create(kiko::g_Renderer, "Space Game", kiko::Color{1, 1, 1, 1});

	m_gameOverText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Impact Label.ttf", 24));
	m_gameOverText->Create(kiko::g_Renderer, "GAME OVER", kiko::Color{1, 1, 1, 1});

	m_deadText = std::make_unique<kiko::Text>(GET_RESOURCE(kiko::Font, "Impact Label.ttf", 24));
	m_deadText->Create(kiko::g_Renderer, "YOU ARE DEAD", kiko::Color{1, 1, 1, 1});


	//load audio
	kiko::g_AudioSystem.AddAudio("laser", "laser.wav");
	kiko::g_AudioSystem.AddAudio("song", "song.wav");

	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("Scene.json");
	m_scene->Initialize();

	//add events
	EVENT_SUBSCRIBE("Addpoints", SpaceGame::AddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", SpaceGame::OnPlayerDead);

	return true;
}

void SpaceGame::Shutdown()
{
}

void SpaceGame::Update(float dt)
{
	switch (state)
	{
	case SpaceGame::eState::Title:		
		m_scene->GetActorByName("Title")->active = true;

		if (kiko::g_InputSystem.GetKeyDown(SDL_SCANCODE_SPACE))
		{
			state = eState::StartGame;
			auto actor = m_scene->GetActorByName("Background");
			if (actor == nullptr)
			{
				WARNING_LOG("Actor doesn't exist")
					break;
			}
			
			m_scene->GetActorByName("Title")->active = false;
		}
		break;
	case SpaceGame::eState::StartGame:
		score = 0;
		lives = 3;
		state = eState::StartLevel;
		break;
	case SpaceGame::eState::StartLevel:
		m_scene->RemoveAll();
		{
			//create player
			auto player = INSTANTIATE(Player, "Player");
			
			//create components
			auto renderComponent = CREATE_CLASS(SpriteComponent);
			renderComponent->m_texture = GET_RESOURCE(kiko::Texture, "pikmin.png", kiko::g_Renderer);
			player->AddComponent(std::move(renderComponent));

			auto physicsComponent = CREATE_CLASS(EnginePhysicsComponent);
			physicsComponent->damping = 0.9f;
			player->AddComponent(std::move(physicsComponent));

			auto collisionComponent = CREATE_CLASS(CircleCollisionComponent);
			collisionComponent->m_radius = 0.0f;
			player->AddComponent(std::move(collisionComponent));

			player->transform = kiko::Transform{ { 400, 300 }, 0, 1 };
			player->Initialize();
			m_scene->Add(std::move(player));

		}
		state = eState::Game;
		break;
	case SpaceGame::eState::Game:
		m_spawnTimer += dt;


		if (m_spawnTimer >= m_spawnTime) {
			m_spawnTimer = 0;
			std::unique_ptr<kiko::Enemy> enemy = std::make_unique<kiko::Enemy>(10.0f, kiko::Pi, kiko::Transform{ {kiko::random(600), kiko::random(600)}, kiko::randomf(kiko::TwoPi), 4 });
			enemy->tag = "Enemy";
			enemy->m_game = this;

			auto renderComponent = CREATE_CLASS(ModelRenderComponent)
				renderComponent->m_model = GET_RESOURCE(kiko::Model, "enemy.txt");
			enemy->AddComponent(std::move(renderComponent));

			auto collisionComponent = CREATE_CLASS(CircleCollisionComponent)
				collisionComponent->m_radius = 30.0f;
			enemy->AddComponent(std::move(collisionComponent));

			enemy->Initialize();
			m_scene->Add(move(enemy));

			
		}

		break;
	case SpaceGame::eState::PlayerDeadStart:
		m_stateTimer = 5.0f;
		if (lives == 0) { state = eState::GameOver; }
		else state = eState::PlayerDead;

		break;
	case SpaceGame::eState::PlayerDead:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0)
		{
			state = eState::StartLevel;
		}


		break;
	case SpaceGame::eState::GameOver:
		m_stateTimer -= dt;
		if (m_stateTimer == 0)
		{
			state = eState::Title;
		}
		break;
	default:
		break;
	}

	m_scoreText->Create(kiko::g_Renderer, "SCORE " + std::to_string(score), { 1,1,1,1 });
	m_livesText->Create(kiko::g_Renderer, "LIVES " + std::to_string(lives), { 1,1,1,1 });
	m_scene->Update(dt);
}

void SpaceGame::Draw(kiko::Renderer& renderer)
{
	m_scene->Draw(renderer);
	if (state == eState::GameOver) {
		m_gameOverText->Draw(renderer, 350, 300);
	}
	if (state == eState::PlayerDead) {
		m_deadText->Draw(renderer, 300, 300);
	}

	m_scoreText->Draw(renderer, 40, 40);
	m_livesText->Draw(renderer, 650, 40);
	kiko::g_particleSystem.Draw(renderer);
}

void SpaceGame::AddPoints(const kiko::Event& event)
{
	score += std::get<int>(event.data);
}

void SpaceGame::OnPlayerDead(const kiko::Event& event)
{
	lives--;
	state = eState::PlayerDeadStart;
}
