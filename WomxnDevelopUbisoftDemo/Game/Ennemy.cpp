#include "stdafx.h"
#include "Ennemy.h"
#include <Game/Player.h>
#include <Engine/GameSpriteLoader.h>


Ennemy::Ennemy(int x, int y,int case_size_x, int case_size_y, Player*p) : Character(sf::Vector2f(static_cast<float>(x * case_size_x + (case_size_x / 2)), static_cast<float>(y)))
{
	m_Radius = 2;
	m_RadiusDetection = m_Radius * case_size_y;
	m_Column = y;
	m_Row = x;
	m_SpeedMax = 200.0f;
	m_Direction = true;
	m_Attack = 25.0f;
	m_MaxLifePoint = 200.0f;
	m_CurrentLifePoint = m_MaxLifePoint;
	m_ToDestroy = false;
	m_TimePreviousAttack = 0.0f;
	m_DurationAttack = 2.0f;

	m_Sprite.setTexture(GameSpriteLoader::TextureEnnemy);
	m_Sprite_Scale = 0.4f;
	m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
	m_Sprite.setOrigin(m_Sprite.getTexture()->getSize().x * 0.5f, 0);

	m_ptr_Player = p;

	m_Clock.restart();
}
Ennemy::Ennemy(const Ennemy& e) : Character(e)
{
	m_Direction = e.m_Direction;
	m_Column = e.m_Column;
	m_Row = e.m_Row;
	m_Radius = e.m_Radius;
	m_RadiusDetection = e.m_RadiusDetection;
	m_ptr_Player = e.m_ptr_Player;
	m_DurationAttack = e.m_DurationAttack;
	m_TimePreviousAttack = e.m_TimePreviousAttack;
	m_Sprite.setTexture(GameSpriteLoader::TextureEnnemy);
	
}
void Ennemy::Update(float deltaTime)
{
	if (IsGrounded())
	{
		m_Velocity.y = 0;
	}
	else
	{
		m_Velocity.y = 600.0f;
	}

	if (m_ptr_Player->IsDead())
	{
		m_HasTarget = false;
	}

	if (IsGrounded() && SeePlayer())
	{
		m_Target = m_ptr_Player->GetPosition();
		m_HasTarget = true;
		
	}
	else if (SeePlayer() == false && m_HasTarget==false)
	{
		m_Target = - m_ptr_Player->GetPosition();
		m_HasTarget = true;
	}

	m_Velocity.x = MoveTo(m_Target, m_SpeedMax).x;

	if (m_Velocity.x > 0.0f) //Right
	{
		FlipSprite(true);
		if (m_Collision[eDirection::Right])
		{
			m_Target = -m_Target;
			FlipSprite(false);
		}	
	}
	else //left
	{
		FlipSprite(false);
		if (m_Collision[eDirection::Left])
		{
			m_Target = -m_Target;
			FlipSprite(true);
		}	
	}
	
	if (m_CurrentLifePoint <= 0.0f)
	{
		m_ToDestroy = true;
	}

	m_Position += m_Velocity * deltaTime;
	m_Sprite.setPosition(m_Position);
	SetBoundingBox(m_Sprite.getGlobalBounds());
}

void Ennemy::OnCollide(Player&)
{
	m_Velocity = sf::Vector2f(0.0f, 0.0f);
	AttackPlayer();
}
void Ennemy::OnCollide(Obstacle& o)
{
	eDirection collisionToBlock = this->CollisionDirection(o);
	if (collisionToBlock != eDirection::Null)
	{
		SetCollision(collisionToBlock);
	}
}

void Ennemy::OnCollide(Platform& p)
{
	eDirection collisionToBlock = this->CollisionDirection(p);
	if (collisionToBlock != -1)
	{
		SetCollision(collisionToBlock);
	}
}

void Ennemy::StartEndGame()
{

}

bool Ennemy::SeePlayer() const
{
	if (m_ptr_Player->IsGhostMode()) return false;

	return (GetPosition().x - m_RadiusDetection < m_ptr_Player->GetPosition().x)
		&& (m_ptr_Player->GetPosition().x < GetPosition().x + m_RadiusDetection);
}
void Ennemy::AttackPlayer()
{
	if (CanAttack() && m_ptr_Player->IsGhostMode() == false)
	{
		m_ptr_Player->LoseLifePoint(m_Attack);
		this->m_TimePreviousAttack = m_Clock.getElapsedTime().asSeconds();
	}
}

bool Ennemy::CanAttack()
{
	return m_Clock.getElapsedTime().asSeconds() > this->m_DurationAttack + this->m_TimePreviousAttack;
}

sf::Vector2f normalize(sf::Vector2f u)
{
	float norm = (float)sqrt(pow(u.x, 2) + pow(u.y, 2)); 
	return sf::Vector2f(u.x / norm, u.y / norm);
}
		
sf::Vector2f Ennemy::MoveTo(sf::Vector2f target, float maxSpeed)
{
	// Seek
	return normalize(target - GetPosition()) * maxSpeed;
}

int Ennemy::getSpawnedColumns() const
{
	return this->m_Column;
}
int Ennemy::getSpawnedRow() const
{
	return this->m_Row;
}
