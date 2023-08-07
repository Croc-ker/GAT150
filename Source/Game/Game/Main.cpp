#include <iostream>
#include "Renderer/Renderer.h"
#include "Core/Core.h"
#include "Renderer/ModelManager.h"
#include "Input/InputSystem.h"
#include "../../Audio/AudioSystem.h"
#include "Core/Time.h"
#include "Framework/Scene.h"
#include "Framework/Resource/ResourceManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Renderer/Text.h"
#include "Renderer/ParticleSystem.h"
#include "SpaceGame.h"
#include <chrono>
#include <vector>
#include <thread>
#include <Core/Logger.h>


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

template <typename T>
void print(const std::string& s, const T& container)
{
	std::cout << s << std::endl;
	for (auto element : container)
	{
		std::cout << element << " ";
	}
	std::cout << std::endl;
}

void print(int count, ...)
{
	va_list args;

	va_start(args, count);
	for (int i = 0; i < count; ++i)
	{
		std::cout << va_arg(args, const char*) << std::endl;
	}
	va_end(args);
}

int main(int argc, char* argv[])
{

	//print(arg, "hello", "world", "goodbye");

	int n[4] = { 1, 2, 3, 4 };
	print("array: ", n);
	cout << n << endl;
	cout << *(n+3) << endl;

	std::array<int, 4> na = { 1, 2, 3, 4 };
	print("array class: ", na);
	cout << na.front() << endl;
	cout << na.back() << endl;
	cout << na.max_size() << endl;


	std::vector<int> nv = { 1, 2, 3, 4 };
	print("vector: ", nv);
	nv.insert(nv.begin() + 2, 0);
	nv.push_back(5);
	nv.pop_back();
	auto iter = nv.erase(nv.begin(), nv.end());
	print("vector: ", nv);

	std::list<int> nl = { 1, 2, 3, 4 };
	print("list: ", nl);
	nl.push_front(0);
	print("list: ", nl);

	std::map<std::string, int> ages;
	ages["charles"] = 17;
	ages["xane"] = 18;
	ages["jacob"] = 19;

	cout << ages["charles"] << endl;


	INFO_LOG("Hello World");

	kiko::MemoryTracker::Initialize();
	kiko::seedRandom((unsigned int)time(nullptr));
	kiko::setFilePath("assets");


	// Initialize Engine
	kiko::g_Renderer.Initialize();
	kiko::g_Renderer.CreateWindow("GAT150", 800, 600);

	kiko::g_InputSystem.Initialize();
	kiko::g_AudioSystem.Initialize();

	unique_ptr<SpaceGame> game = make_unique<SpaceGame>();
	game->Initialize();


	vector<Star> stars;
	for (int i = 0; i < 1000; i++) {
		kiko::Vector2 pos(kiko::random(kiko::g_Renderer.GetWidth()), kiko::random(kiko::g_Renderer.GetHeight()));
		kiko::Vector2 vel(kiko::randomf(100, 40), 0.0f);

		stars.push_back(Star(pos, vel));
	}

	shared_ptr<kiko::Texture> texture = kiko::g_resources.Get<kiko::Texture>("ship.png", kiko::g_Renderer);
	//texture->Load("fnaf2.png", kiko::g_Renderer);

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
		kiko::g_Renderer.DrawTexture(texture.get(), 200.0f, 200.0f, 0.0f);
		game->Draw(kiko::g_Renderer);

		//text->Draw(kiko::g_Renderer, 400, 300);

		kiko::g_Renderer.EndFrame();
	}

	stars.clear();

	return 0;
}
