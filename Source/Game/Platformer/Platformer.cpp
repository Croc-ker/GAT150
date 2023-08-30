#include "Platformer.h"

#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/ModelManager.h"


bool Platformer::Initialize()
{
	//load audio
	//kiko::g_AudioSystem.AddAudio("laser", "laser-gun.wav");
	//kiko::g_AudioSystem.AddAudio("music", "music.wav");

	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("Scenes/Scene.json");
	m_scene->Load("Scenes/tilemap.json");
	m_scene->Initialize();

	/*std::vector<Enemy> enemies;
	for (int i = 0; i < 5; i++) {
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(10.0f, kiko::Pi, kiko::Transform{ {kiko::random(600), kiko::random(600)}, kiko::randomf(kiko::TwoPi), 3 }, kiko::g_manager.Get("ship.txt"));
		enemy->m_tag = "Enemy";
		m_scene->Add(move(enemy));
	}*/

	EVENT_SUBSCRIBE("AddPoints", Platformer::AddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", Platformer::OnPlayerDead);

	//kiko::EventManager::Instance().Subscribe("AddPoints", this, std::bind(&Platformer::AddPoints, this, std::placeholders::_1));
	//kiko::EventManager::Instance().Subscribe("OnPlayerDead", this, std::bind(&Platformer::OnPlayerDead, this, std::placeholders::_1));

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
		/*auto actor = INSTANTIATE(Actor, "Crate");
		actor->transform.position = { kiko::random(kiko::g_Renderer.GetWidth()),100 };
		actor->Initialize();
		m_scene->Add(std::move(actor));*/
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