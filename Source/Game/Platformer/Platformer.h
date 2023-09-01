#pragma once
#pragma once
#include "Framework/Game.h"
#include "Framework/Event/EventManager.h"


class Platformer : public kiko::Game, kiko::IEventListener
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

	void SetState(eState state) { m_state = state; }
	void AddPoints(const kiko::Event& event);
	void OnPlayerDead(const kiko::Event& event);
	void RandomizeScene();

private:
	eState m_state = eState::Title;
	float m_spawnTimer = 0;
	float m_spawnTime = 3.0f;

	float m_stateTimer = 0;
};