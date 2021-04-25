#pragma once
#include "Character.h"

class Player;

class Ennemy : public Character
{
public:
	
	Ennemy(int x, int y, int case_size_x, int case_size_y, Player*);
	Ennemy(const Ennemy&);

	virtual void Update(float deltaTime);
	virtual void StartEndGame();
	virtual void OnCollide(Platform&);
	virtual void OnCollide(Obstacle&);
	void OnCollide(Player&);
	
	bool SeePlayer() const;
	int getSpawnedColumns() const;
	int getSpawnedRow() const;

	
	
private:

	bool m_Direction;
	float m_DurationAttack,m_TimePreviousAttack;
    float m_Radius,m_RadiusDetection;
	int m_Column, m_Row;
	Player* m_ptr_Player;

	void AttackPlayer();
	bool CanAttack();
	
	sf::Vector2f MoveTo(sf::Vector2f target,float maxSpeed);

public:
	bool operator==(const Ennemy& e)
	{
		return (this->m_Position == e.m_Position && this->m_CurrentLifePoint == e.m_CurrentLifePoint);
	}
	
};

