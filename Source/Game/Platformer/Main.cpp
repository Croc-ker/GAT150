#include <iostream>
#include "Platformer.h"
#include "Renderer/Renderer.h"
#include "Core/Core.h"
#include "Renderer/Model.h"
#include "Input/InputSystem.h"
#include "../../Physics/PhysicsSystem.h"
#include "../../Audio/AudioSystem.h"
#include "Core/Time.h"
#include "Renderer/Text.h"
#include "Renderer/ParticleSystem.h"
#include <chrono>
#include <vector>
#include <thread>
#include <Core/Logger.h>
#include "Framework/Framework.h"
#include <functional>

using namespace std;

int main(int argc, char* argv[])
{
	kiko::Factory::Instance().Register<kiko::SpriteComponent>("SpriteComponent");
	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");

	INFO_LOG("Starting game: Space Game");

	// Initialize Engine
	kiko::g_Renderer.Initialize();
	kiko::g_Renderer.CreateWindow("GAT150", 800, 600);

	kiko::g_InputSystem.Initialize();
	kiko::g_AudioSystem.Initialize();
	kiko::PhysicsSystem::Instance().Initialize();

	unique_ptr<Platformer> game = make_unique<Platformer>();
	game->Initialize();

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
		game->Update(kiko::g_time.GetDeltaTime());
		kiko::g_AudioSystem.Update();
		kiko::g_particleSystem.Update(kiko::g_time.GetDeltaTime());
		kiko::PhysicsSystem::Instance().Update(kiko::g_time.GetDeltaTime());

		//draw game
		kiko::g_Renderer.SetColor(0, 0, 0, 0);
		kiko::g_Renderer.BeginFrame();
		game->Draw(kiko::g_Renderer);
		kiko::g_Renderer.EndFrame();
	}
	return 0;
}
