#pragma once
#include <memory>

namespace kiko
{
	class Scene;
	class Renderer;

	class Game
	{
	public:
		Game() = default;
		virtual ~Game() = default;

		virtual bool Initialize() = 0;
		virtual void Shutdown() = 0;

		virtual void Update(float dt) = 0;
		virtual void Draw(Renderer& renderer) = 0;

		int GetScore() const { return score; }
		void AddPoints(int points) { score += points; }

		int GetLives() const { return lives; }
		void SetLives(int lives) { lives = lives; }

	protected:
		std::unique_ptr<Scene> m_scene;

		int score = 0;
		int lives = 0;
		int m_health = 0;
	};
}

