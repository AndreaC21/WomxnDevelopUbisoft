#pragma once
#include "Game/Displayable.h"
#include "Game/Player.h"
#include <typeinfo>

class Ennemy : public Displayable
{
public:
	Ennemy();
	Ennemy(sf::Vector2f position,Player* p);
	Ennemy(const Ennemy&);

	virtual void Update(float deltaTime);
	virtual void StartEndGame();

	
	std::string getLifePoint() const;
	float getCurrentLifePoint() const;
	void lostLifePoint(float amount);

	bool SeePlayer() const;
	
private:

	Player* m_ptr_Player;
	sf::Vector2f m_Velocity;
	bool m_Direction,m_ToDestroy;
	float m_lifePoint_max, m_lifePoint, m_attack;

	void MoveTo(sf::Vector2f target);
	
};

