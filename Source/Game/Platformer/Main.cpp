#include <iostream>
#include "Core/Core.h"
#include "Renderer/Renderer.h"
#include "Framework/Framework.h"
#include "box2d/include/box2d/box2d.h"
#include "box2d/include/box2d/b2_world.h"
#include "Renderer/ModelManager.h"
#include "Input/InputSystem.h"
#include "Audio/AudioSystem.h"
#include "Physics/PhysicsSystem.h"
#include "Platformer.h"
#include <chrono>
#include <vector>
#include <thread>
#include <functional>


using namespace std;

int main(int argc, char* argv[])
{

	INFO_LOG("hello world");

	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");


	// Initialize Engine
	kiko::g_Renderer.Initialize();
	kiko::g_Renderer.CreateWindow("CSC196", 800, 600);

	kiko::g_InputSystem.Initialize();
	kiko::g_AudioSystem.Initialize();
	kiko::PhysicsSystem::Instance().Initialize();

	unique_ptr<Platformer> game = make_unique<Platformer>();
	game->Initialize();


	kiko::g_AudioSystem.PlayOneShot("music", true);
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
		kiko::PhysicsSystem::Instance().Update(kiko::g_time.GetDeltaTime());

		//draw game
		kiko::g_Renderer.SetColor(0, 0, 0, 0);
		kiko::g_Renderer.BeginFrame();
		//draw
		kiko::Vector2 vel(1.0f, 0.3f);

		game->Draw(kiko::g_Renderer);


		//	text->Draw(kiko::g_Renderer, 400, 300);

		kiko::g_Renderer.EndFrame();
	}
	return 0;
}

//class not found in factory: tile01
//this happens because the tile01 is not registered in the factory, which happens within the tilemap class