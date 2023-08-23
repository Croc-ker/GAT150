#include <iostream>
#include "Renderer/Renderer.h"
#include "Core/Core.h"
#include "Renderer/Model.h"
#include "Input/InputSystem.h"
#include "../../Physics/PhysicsSystem.h"
#include "../../Audio/AudioSystem.h"
#include "Core/Time.h"
#include "Player.h"
#include "Enemy.h"
#include "Renderer/Text.h"
#include "Renderer/ParticleSystem.h"
#include "SpaceGame.h"
#include <chrono>
#include <vector>
#include <thread>
#include <Core/Logger.h>
#include "Framework/Framework.h"
#include <functional>

using namespace std;

class Star
{
public:
	Star(const kiko::vec2& pos, const kiko::vec2& vel) :
		m_pos{ pos },
		m_vel{ vel }
	{}

	void Update(int width, int height)
	{
		m_pos += m_vel * kiko::g_time.GetDeltaTime();
		if (m_pos.x > width) {
			m_pos.x = 0;
		}
		if (m_pos.y > height) {
			m_pos.y = 0;
		}
	}

	void Draw(kiko::Renderer& renderer)
	{
		renderer.DrawPoint(m_pos.x, m_pos.y);
	}

public:
	kiko::vec2 m_pos;
	kiko::vec2 m_vel;
};

int main(int argc, char* argv[])
{


	INFO_LOG("Starting game: Space Game");

	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");


	rapidjson::Document document;
	kiko::Json::Load("json.txt", document);

	kiko::Factory::Instance().Register<kiko::SpriteComponent>("SpriteComponent");

	// Initialize Engine
	kiko::g_Renderer.Initialize();
	kiko::g_Renderer.CreateWindow("GAT150", 800, 600);

	kiko::g_InputSystem.Initialize();
	kiko::g_AudioSystem.Initialize();
	kiko::PhysicsSystem::Instance().Initialize();

	unique_ptr<SpaceGame> game = make_unique<SpaceGame>();
	game->Initialize();


	vector<Star> stars;
	for (int i = 0; i < 1000; i++) {
		kiko::Vector2 pos(kiko::random(kiko::g_Renderer.GetWidth()), kiko::random(kiko::g_Renderer.GetHeight()));
		kiko::Vector2 vel(kiko::randomf(100, 40), 0.0f);

		stars.push_back(Star(pos, vel));
	}

	/*shared_ptr<kiko::Texture> texture = GET_RESOURCE(kiko::Texture, "ship.png", kiko::g_Renderer);
	texture->Load("ship.png", kiko::g_Renderer);*/

	//main game loop
	bool quit = false;
	while (!quit)
	{
		//update engine
		kiko::g_time.Tick();
		kiko::g_InputSystem.Update();
		if (kiko::g_InputSystem.GetKeyDown(SDL_SCANCODE_ESCAPE))
		{
			quit = true;
		}
		//update
		kiko::g_AudioSystem.Update();


		game->Update(kiko::g_time.GetDeltaTime());

		kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());

		//draw game
		kiko::g_Renderer.SetColor(0, 0, 0, 0);
		kiko::g_Renderer.BeginFrame();
		//draw
		kiko::Vector2 vel(1.0f, 0.3f);

		for(auto& star : stars){
			star.Update(kiko::g_Renderer.GetWidth(), kiko::g_Renderer.GetHeight());


			kiko::g_Renderer.SetColor(kiko::random(256), kiko::random(256), kiko::random(256), 255);
			star.Draw(kiko::g_Renderer);
		}
		
		game->Draw(kiko::g_Renderer);

		//text->Draw(kiko::g_Renderer, 400, 300);

		kiko::g_Renderer.EndFrame();
	}

	stars.clear();

	return 0;
}
