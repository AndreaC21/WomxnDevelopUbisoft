#pragma once
#include "Displayable.h"
//#include "Player.h"


class Ennemy : public Displayable
{
public:
	Ennemy();
	//Ennemy(sf::Vector2f position,Player* p);
	Ennemy(sf::Vector2f position);
	Ennemy(int x, int y, int case_size_x, int case_size_y);
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
	
private:

	//Player* m_ptr_Player;
	sf::Vector2f m_Velocity;
	bool m_Direction, m_ToDestroy, m_OnGround;
	float m_lifePoint_max, m_lifePoint, m_attack,m_radius,m_RadiusDetection;
	int m_Column;

	void MoveTo(sf::Vector2f target);

public:
	bool operator==(const Ennemy& e)
	{
		return (this->m_Position == e.m_Position &&
			this->Dead() == e.Dead());

	}
	
};

