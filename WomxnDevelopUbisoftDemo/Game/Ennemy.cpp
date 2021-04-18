#include "stdafx.h"
#include "Ennemy.h"
#include <Game/Player.h>


Ennemy::Ennemy(int x, int y,int case_size_x, int case_size_y, Player*p) : Displayable(sf::Vector2f(static_cast<float>(x * case_size_x + (case_size_x / 2)), static_cast<float>(y)), "Ennemy\\Idle_1.png")
{
	m_radius = 2;
	m_RadiusDetection = m_radius * case_size_y;
	m_Column = y;
	m_Row = x;

	m_Direction = true;
	m_attack = 25.0f;
	m_lifePoint_max = 200.0f;
	m_lifePoint = m_lifePoint_max;
	m_ToDestroy = false;
	m_OnGround = false;
	m_TimePreviousAttack = 0.0f;
	m_DurationAttack = 2.0f;

	m_Sprite_Scale = 0.4f;
	m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);

	const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));
	SetBoundingBox(m_Position.x, m_Position.y, size.x * m_Sprite_Scale, size.y * m_Sprite_Scale);

	m_ptr_Player = p;

	clock.restart();
}
Ennemy::Ennemy(const Ennemy& e) : Displayable(e)
{
	m_Direction = e.m_Direction;
	m_attack = e.m_attack;
	m_lifePoint_max = e.m_lifePoint_max;
	m_lifePoint = e.m_lifePoint;
	m_ToDestroy = e.m_ToDestroy;
	m_Column = e.m_Column;
	m_Row = e.m_Row;
	m_OnGround = e.m_OnGround;
	m_radius = e.m_radius;
	m_RadiusDetection = e.m_RadiusDetection;
	m_ptr_Player = e.m_ptr_Player;
	m_DurationAttack = e.m_DurationAttack;
	m_TimePreviousAttack = e.m_TimePreviousAttack;

	m_Sprite_Scale = e.m_Sprite_Scale;
	m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
}
void Ennemy::Update(float deltaTime)
{
	const float SPEED_MAX = 200.0f;
	
	if (m_OnGround)
	{
		m_Velocity.y = 0;
	}

	if (m_OnGround && SeePlayer())
	{
		m_Velocity.x = MoveTo(m_ptr_Player->getPosition(),SPEED_MAX).x;

		if (IsColliding(*m_ptr_Player->getCurrentState()))
		{
			m_Velocity = sf::Vector2f(0.0f, 0.0f);
			AttackPlayer();
		}
	}
	else if ( SeePlayer()==false)m_Velocity = sf::Vector2f(0.0f, 0.0f);
	

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
	if (m_ptr_Player->isGhostMode()) return false;

	return (getPosition().x - m_RadiusDetection < m_ptr_Player->getPosition().x)
		&& (m_ptr_Player->getPosition().x < getPosition().x + m_RadiusDetection);
}
void Ennemy::AttackPlayer()
{
	if (CanAttack() && m_ptr_Player->isGhostMode() == false)
	{
		Explorator* e = static_cast<Explorator*>(m_ptr_Player->getCurrentState());
		e->loseLifePoint(m_attack);
		this->m_TimePreviousAttack = clock.getElapsedTime().asSeconds();
	}
}

bool Ennemy::CanAttack()
{
	return clock.getElapsedTime().asSeconds() > this->m_DurationAttack + this->m_TimePreviousAttack;
}

sf::Vector2f normalize(sf::Vector2f u)
{
	float norm = (float)sqrt(pow(u.x, 2) + pow(u.y, 2)); 
	return sf::Vector2f(u.x / norm, u.y / norm);
}
		
sf::Vector2f Ennemy::MoveTo(sf::Vector2f target, float maxSpeed)
{
	// Seek
	return normalize(target - getPosition()) * maxSpeed;
}

int Ennemy::getSpawnedColumns() const
{
	return this->m_Column;
}
int Ennemy::getSpawnedRow() const
{
	return this->m_Row;
}
