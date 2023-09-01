#include "Platformer.h"
#include "Player.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/ModelManager.h"

bool Platformer::Initialize()
{
	//load audio

	m_scene = std::make_unique<kiko::Scene>();
	RandomizeScene();
	/*m_scene->Load("Scenes/Scene3.json");
	m_scene->Load("Scenes/tilemap3.json");
	m_scene->Initialize();*/
	EVENT_SUBSCRIBE("AddPoints", Platformer::AddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", Platformer::OnPlayerDead);


	return true;
}

void Platformer::Shutdown()
{
}

void Platformer::Update(float dt)
{
	switch (m_state)
	{
	case Platformer::eState::Title:
	{
	}
	break;
	case Platformer::eState::StartGame:
		m_score = 0;
		m_lives = 3;
		m_state = eState::StartLevel;
		break;
	case Platformer::eState::StartLevel:
		m_scene->RemoveAll();
		m_state = eState::Game;
		break;
	case Platformer::eState::Game:
	{
		auto player = m_scene->GetActorByName("Player");
		auto coin = m_scene->GetActorByName("Coin");
		if (player->OnCollisionEnter(coin)) {
			m_state = eState::StartLevel;
			RandomizeScene();
		}
	}
		break;
	case Platformer::eState::PlayerDeadStart:

		if (m_lives == 0) { m_state = eState::GameOver; }
		else m_state = eState::PlayerDead;

		break;
	case Platformer::eState::PlayerDead:
		m_stateTimer -= dt;
		if (m_stateTimer <= 0)
		{
			m_state = eState::StartLevel;
		}


		break;
	case Platformer::eState::GameOver:
		m_stateTimer -= dt;
		if (m_stateTimer == 0)
		{
			m_state = eState::Title;
		}
		break;
	default:
		break;
	}
	m_scene->Update(dt);
}

void Platformer::Draw(kiko::Renderer& renderer)
{
	m_scene->Draw(renderer);

	kiko::g_particleSystem.Draw(renderer);
}

void Platformer::AddPoints(const kiko::Event& event)
{
	m_score += std::get<int>(event.data);
}

void Platformer::OnPlayerDead(const kiko::Event& event)
{
	m_lives--;
	m_state = eState::PlayerDeadStart;
}

void Platformer::RandomizeScene()
{
	m_scene->RemoveAll();
	int random = kiko::random(4) + 1;
	switch (random) {
	case 1:
		m_scene->Load("Scenes/Scene.json");
		m_scene->Load("Scenes/tilemap.json");
		break;
	case 2:
		m_scene->Load("Scenes/Scene2.json");
		m_scene->Load("Scenes/tilemap2.json");
		break;
	case 3:
		m_scene->Load("Scenes/Scene3.json");
		m_scene->Load("Scenes/tilemap3.json");
		break;
	default:
		m_scene->Load("Scenes/Scene.json");
		m_scene->Load("Scenes/tilemap.json");
	}
	m_scene->Initialize();
}

