#pragma once
#include "Core/Core.h"
#include "Renderer/Model.h"
#include <memory>
#include "Components/Component.h"


namespace kiko
{
	class Component;

	class Actor
	{
	public:
		Actor() = default;
		Actor(const kiko::Transform& transform, std::shared_ptr<Model> model) :
			m_transform{ transform },
			m_model{ model }
		{}
		Actor(const kiko::Transform& transform) :
			m_transform{ transform }
		{}

		virtual void Update(float dt);
		virtual void Draw(kiko::Renderer& renderer);

		void AddComponent(std::unique_ptr<Component>(component));

		template<typename T>
		T* GetComponent();

		float GetRadius() { return 30.0f; /*(m_model) ? m_model->GetRadius() * m_transform.scale : 0; */ }
		virtual void OnCollision(Actor* other) {}

		void Addforce(vec2 force) { m_velocity += force; }
		void SetDamping(float damping) { m_damping = damping; }

		float Get
		() const { return m_lifespan; }
		void SetLifespan(float lifespan) { m_lifespan = lifespan; }


		class Scene* m_scene = nullptr;
		friend class Scene;
		friend class Game;

		kiko::Transform m_transform;
		std::string m_tag;

		class Game* m_game = nullptr;

	protected:
		std::vector<std::unique_ptr<Component>> m_components;

		bool m_destroyed = false;
		float m_lifespan = -1.0f;

		vec2 m_velocity;
		float m_damping = 0;

		std::shared_ptr<Model> m_model;
		
	};
	template<typename T>
	inline T* Actor::GetComponent()
	{
		for (auto& component : m_components) {
			T* result = dynamic_cast<T*>(component.get());
			if (result) return result;
		}
		return nullptr;
	}
}