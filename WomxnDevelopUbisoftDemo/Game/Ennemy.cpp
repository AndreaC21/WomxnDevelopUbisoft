#include "stdafx.h"
#include "Ennemy.h"

Ennemy::Ennemy()
{

}
/*
Ennemy::Ennemy(sf::Vector2f position,Player *p) : Displayable(position, "Ennemy\\Idle_1.png")
{
	m_Direction = true;
	m_attack = 100.0f;
	m_lifePoint_max = 200.0f;
	m_lifePoint = m_lifePoint_max;
	m_ptr_Player = p;
	m_ToDestroy = false;

	m_Sprite_Scale = 0.4f;
	m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);

	const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));
	SetBoundingBox(m_Position.x, m_Position.y, size.x * m_Sprite_Scale, size.y * m_Sprite_Scale);
	
}*/
Ennemy::Ennemy(sf::Vector2f position) : Displayable(position, "Ennemy\\Idle_1.png")
{
	m_Direction = true;
	m_attack = 100.0f;
	m_lifePoint_max = 200.0f;
	m_lifePoint = m_lifePoint_max;
	m_ToDestroy = false;
	m_OnGround = false;

	m_Sprite_Scale = 0.4f;
	m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);

	const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));
	SetBoundingBox(m_Position.x, m_Position.y, size.x * m_Sprite_Scale, size.y * m_Sprite_Scale);
}
Ennemy::Ennemy(int x, int y,int case_size_x, int case_size_y) : Ennemy(sf::Vector2f(static_cast<float>(x * case_size_x + (case_size_x / 2)), static_cast<float>(y)))
{
	m_radius = 2;
	m_radius = m_radius * case_size_y;
	m_Column = y;
	
}
Ennemy::Ennemy(const Ennemy& e) : Displayable(e)
{
	m_Direction = e.m_Direction;
	m_attack = e.m_attack;
	m_lifePoint_max = e.m_lifePoint_max;
	m_lifePoint = e.m_lifePoint;
	//m_ptr_Player = e.m_ptr_Player;
	m_ToDestroy = e.m_ToDestroy;
	m_Column = e.m_Column;
	m_OnGround = e.m_OnGround;

	m_Sprite_Scale = e.m_Sprite_Scale;
	m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
}
void Ennemy::Update(float deltaTime)
{
	if (m_OnGround == false)
	{
		m_Velocity.y = 200.0f;
	}
	else
	{
		m_Velocity.y = 0;
	}

	if (m_lifePoint <= 0.0f)
	{
		m_ToDestroy = true;
	}

	m_Position += m_Velocity * deltaTime;
	m_Sprite.setPosition(m_Position);
	SetCenter(m_Position.x, m_Position.y);
}

bool Ennemy::isGrounded() const
{
	return this->m_OnGround;
}

void Ennemy::Fall()
{
	m_Velocity.y = 600.0f;
}
void Ennemy::StopFall()
{
	m_Velocity.y = 0;
	this->m_OnGround = true;

}

void Ennemy::StartEndGame()
{

}

std::string Ennemy::getLifePoint() const
{
	return std::to_string(m_lifePoint) + "/" + std::to_string(m_lifePoint_max);
}

void Ennemy::lostLifePoint(float amount)
{
	this->m_lifePoint -= amount;
}
float Ennemy::getCurrentLifePoint() const
{
	return this->m_lifePoint;
}

bool Ennemy::Dead() const
{
	return m_lifePoint <= 0.0f;
}

bool Ennemy::SeePlayer() const
{
	return false;
}

void Ennemy::MoveTo(sf::Vector2f target)
{

}

int Ennemy::getSpawnedColumns() const
{
	return this->m_Column;
}
