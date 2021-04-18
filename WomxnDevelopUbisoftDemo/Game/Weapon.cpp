#include "stdafx.h"
#include "Weapon.h"


Weapon::Weapon()
{

}
Weapon::Weapon(const Weapon& w) : Displayable(w)
{
    m_force = w.m_force;
    m_duration = w.m_duration;
    m_Sprite_Scale = 0.5f;
    
    m_Direction = w.m_Direction;
    m_ToDestroy = w.m_ToDestroy;
    m_TimeStart = w.m_TimeStart;
    m_TimeEnd = w.m_TimeEnd;

    if (m_Direction)
        m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
    else
        m_Sprite.setScale(-m_Sprite_Scale, m_Sprite_Scale);

    SetBoundingBox(m_Sprite.getGlobalBounds());
}
Weapon::Weapon(sf::Vector2f position, bool direction, float timeStart) : Displayable(position, "Weapon\\Kunai.png")
{
	m_force = 500.0f;
	m_duration = 2.0f;
	m_Sprite_Scale = 0.5f;
   
    m_Direction = direction;
    if (m_Direction)
        m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
    else
        m_Sprite.setScale(-m_Sprite_Scale, m_Sprite_Scale);
    m_ToDestroy = false;
    m_TimeStart = timeStart;
    m_TimeEnd = m_TimeStart + m_duration;
    SetBoundingBox(m_Sprite.getGlobalBounds());

    clock.restart();

}

void Weapon::Update(float deltaTime)
{
    const float SPEED_MAX = 500.0f;
    const float SPEED_INC = 50.0f;

    if (clock.getElapsedTime().asSeconds() >= m_duration)
    {
        m_ToDestroy = true;
    }
    else
    {
        if (m_Direction) // Right
        {
            m_Velocity.x = fmin(m_Velocity.x + SPEED_INC, SPEED_MAX);
        }
        else // LEFT
        {
            m_Velocity.x = fmax(m_Velocity.x - SPEED_INC, -SPEED_MAX);
        }
    }
    m_Position += m_Velocity * deltaTime;

    SetCenter(m_Position.x, m_Position.y);
  
}

bool Weapon::TouchDisplayable(Displayable* d)
{
   
    if ( typeid(*d) == typeid(Obstacle) || typeid(*d) == typeid(Platform))
    {
       m_ToDestroy = true;
    }
    return true;
}

bool Weapon::TouchEnnemy(Ennemy& e)
{
    m_ToDestroy = true;
    e.lostLifePoint(this->m_force);
    return true;
}
   
void Weapon::StartEndGame()
{

}

bool Weapon::Finish()
{
    return this->m_ToDestroy;
}
