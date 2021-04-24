#include "stdafx.h"
#include "Weapon.h"


Weapon::Weapon() : Character()
{
    m_Duration = 0;
    m_TimeEnd = 0.0;
    m_TimeStart = 0.0f;
    m_SpeedInc = 0.0f;
}
Weapon::Weapon(const Weapon& w) : Character(w)
{
    m_Duration = w.m_Duration;
    m_Sprite_Scale = 0.5f;
    m_SpeedInc = w.m_SpeedInc;
    m_TimeStart = w.m_TimeStart;
    m_TimeEnd = w.m_TimeEnd;

    if (m_SpeedMax > 0)
        m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
    else
        m_Sprite.setScale(-m_Sprite_Scale, m_Sprite_Scale);

}
Weapon::Weapon(sf::Vector2f position, bool direction, float timeStart) : Character(position, "Weapon\\Kunai.png")
{
	m_Attack = 500.0f;
	m_Duration = 2.0f;
	m_Sprite_Scale = 0.5f;
    m_SpeedMax = (direction == true) ? 500.0f: -500.0f;
    m_SpeedInc = 10.0f;
   
    if (m_SpeedMax > 0)
        m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
    else
        m_Sprite.setScale(-m_Sprite_Scale, m_Sprite_Scale);
    m_ToDestroy = false;
    m_TimeStart = timeStart;
    m_TimeEnd = m_TimeStart + m_Duration;
    SetBoundingBox(m_Sprite.getGlobalBounds());

    m_Clock.restart();
}

void Weapon::Update(float deltaTime)
{
    if (m_Clock.getElapsedTime().asSeconds() >= m_Duration)
    {
        m_ToDestroy = true;
    }
    else
    {
        m_Velocity.x = fmin(m_Velocity.x + m_SpeedInc, m_SpeedMax);
       
    }
    m_Position += m_Velocity * deltaTime;
    m_Sprite.setPosition(m_Position);
    SetBoundingBox(m_Sprite.getGlobalBounds());
}

void Weapon::OnCollide(Obstacle&)
{
    m_ToDestroy = true;
}
void Weapon::OnCollide(Ennemy& e)
{
    e.LoseLifePoint(this->m_Attack);
    m_ToDestroy = true;
}
void Weapon::OnCollide(Platform&)
{
    m_ToDestroy = true;
}
bool Weapon::ToDestroy() const
{
    return m_ToDestroy;
}
void Weapon::StartEndGame()
{

}
