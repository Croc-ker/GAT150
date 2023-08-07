#pragma once
#include "Framework/Actor.h"

namespace kiko
{
	class Component
	{
	public:
		virtual void Update(float dt) = 0;

		friend class Actor;

		Actor* m_owner = nullptr;

	private:

	};
}