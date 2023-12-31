#pragma once
#include "Framework/Game.h"
#include "Renderer/Text.h"
#include "Framework/Event/EventManager.h"

class SpaceGame : public kiko::Game, kiko::IEventListener
{
public:
	enum class eState
	{
		Title,
		StartGame,
		StartLevel,
		Game,
		PlayerDeadStart,
		PlayerDead,
		GameOver
	};

public:

	virtual bool Initialize() override;
	virtual void Shutdown() override;

	virtual void Update(float dt) override;
	virtual void Draw(kiko::Renderer& renderer) override;

	void SetState(eState state) { state = state; }
	void AddPoints(const kiko::Event& event);
	void OnPlayerDead(const kiko::Event& event);

private:
	eState state = eState::Title;
	float m_spawnTimer = 0.0f;
	float m_spawnTime = 3.0f;

	float m_stateTimer = 0.0f;

	std::shared_ptr<kiko::Font> m_font;
	std::unique_ptr<kiko::Text> m_scoreText;
	std::unique_ptr<kiko::Text> m_titleText;
	std::unique_ptr<kiko::Text> m_gameOverText;
	std::unique_ptr<kiko::Text> m_livesText;
	std::unique_ptr<kiko::Text> m_deadText;
};