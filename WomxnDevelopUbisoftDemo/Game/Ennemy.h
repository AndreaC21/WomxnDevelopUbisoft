#pragma once
#include "Displayable.h"

class Player;

class Ennemy : public Displayable
{
public:
	
	Ennemy(int x, int y, int case_size_x, int case_size_y, Player*);
	Ennemy(const Ennemy&);

	virtual void Update(float deltaTime);
	virtual void StartEndGame();
	virtual bool ToDestroy();

	
	std::string getLifePoint() const;
	float getCurrentLifePoint() const;
	void lostLifePoint(float amount);

	bool SeePlayer() const;
	bool isGrounded() const;
	void Fall();
	void StopFall();

	int getSpawnedColumns() const;
	int getSpawnedRow() const;

	Player* m_ptr_Player;
	
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
		return (this->m_Position == e.m_Position && this->m_lifePoint == e.m_lifePoint);
	}
	
};

