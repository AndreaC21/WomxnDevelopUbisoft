#pragma once
#include "Displayable.h"


class Ennemy : public Displayable
{
public:
	
	Ennemy(sf::Vector2f position);
	Ennemy(int x, int y, int case_size_x, int case_size_y, Displayable*);
	Ennemy(const Ennemy&);

	virtual void Update(float deltaTime);
	virtual void StartEndGame();

	
	std::string getLifePoint() const;
	float getCurrentLifePoint() const;
	void lostLifePoint(float amount);

	bool SeePlayer() const;
	bool Dead() const;
	bool isGrounded() const;
	void Fall();
	void StopFall();

	int getSpawnedColumns() const;
	int getSpawnedRow() const;

	Displayable* m_ptr_Player;
	
private:

	sf::Vector2f m_Velocity;
	bool m_Direction, m_ToDestroy, m_OnGround;
	float m_DurationAttack,m_TimePreviousAttack;
	float m_lifePoint_max, m_lifePoint, m_attack,m_radius,m_RadiusDetection;
	int m_Column,m_Row;
	sf::Clock clock;

	void AttackPlayer();
	bool CanAttack();
	
	sf::Vector2f MoveTo(sf::Vector2f target,float maxSpeed);

public:
	bool operator==(const Ennemy& e)
	{
		return (this->m_Position == e.m_Position && this->Dead() == e.Dead());
	}
	
};

