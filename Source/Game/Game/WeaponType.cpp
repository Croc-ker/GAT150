#include "WeaponType.h"
#include "Player.h"
#include "../../Audio/AudioSystem.h"
void WeaponType::Update(float dt)
{
    Actor::Update(dt);
}
void WeaponType::OnCollision(Actor* other)
{
    if (other->m_tag == "Player")
    {
        switch (m_type)
        {
            case Type::NormalShot:
                GiveNormalShot(other);
                break;
            case Type::DoubleShot:
                GiveDoubleShot(other);
                break;
            case Type::BigShot:
                GiveBigShot(other);
                break;
        }
        kiko::g_AudioSystem.PlayOneShot("pickup");
        m_destroyed = true;
    }
}

void WeaponType::GiveNormalShot(Actor* player) {
    dynamic_cast<Player*>(player)->SetWeaponType(Type::NormalShot);
}
void WeaponType::GiveDoubleShot(Actor* player) {
    dynamic_cast<Player*>(player)->SetWeaponType(Type::DoubleShot);
}
void WeaponType::GiveBigShot(Actor* player) {
    dynamic_cast<Player*>(player)->SetWeaponType(Type::BigShot);
}

void WeaponType::SetRandomType()
{
    int randnum = kiko::random(0,2);
    WeaponType::Type tempType;
    switch (randnum)
    {
    case 0:
        m_type = NormalShot;
        break;
    case 1:
        m_type = DoubleShot;
        break;
    case 2:
        m_type = BigShot;
        break;
    default:
        m_type = NormalShot;
    }
}
