#include "Platformer.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "../../Audio/AudioSystem.h"
#include "Input/InputSystem.h"
#include "Renderer/ModelManager.h"
#include "Framework/Actor.h"



bool Platformer::Initialize()
{

	//load audio
	kiko::g_AudioSystem.AddAudio("song", "song.wav");

	//load scene
	m_scene = std::make_unique<kiko::Scene>();
	m_scene->Load("Scenes/Scene.json");
	m_scene->Initialize();

	//add events
	EVENT_SUBSCRIBE("Addpoints", Platformer::AddPoints);
	EVENT_SUBSCRIBE("OnPlayerDead", Platformer::OnPlayerDead);

	return true;
}

void Platformer::Shutdown()
{
}

void Platformer::Update(float dt)
{
	switch (state)
	{
	case Platformer::eState::Title:

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
	case Platformer::eState::StartGame:
		state = eState::StartLevel;
		break;
	case Platformer::eState::StartLevel:
		m_scene->RemoveAll();
		state = eState::Game;
		break;
	case Platformer::eState::Game:
		kiko::PhysicsSystem::Instance().Update(kiko::g_time.GetDeltaTime());
		break;
	case Platformer::eState::PlayerDeadStart:
		break;
	case Platformer::eState::PlayerDead:
		break;
	case Platformer::eState::GameOver:
		break;
	default:
		break;
	}
	m_scene->Update(dt);
}

void Platformer::Draw(kiko::Renderer& renderer)
{
	m_scene->Draw(renderer);
}

void Platformer::AddPoints(const kiko::Event& event)
{
	score += std::get<int>(event.data);
}

void Platformer::OnPlayerDead(const kiko::Event& event)
{
	lives--;
	state = eState::PlayerDeadStart;
}
