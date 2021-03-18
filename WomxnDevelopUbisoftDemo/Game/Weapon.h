#pragma once
#include "Displayable.h"
class Weapon : public Displayable
{
public:
	Weapon();
	Weapon(const Weapon& w);
	Weapon(sf::Vector2f position,bool direction, float timeStart);

	virtual void Update(float deltaTime);
	virtual void StartEndGame();

	bool Finish();

private:
	float m_force,m_duration,m_TimeStart,m_TimeEnd;
	sf::Clock clock;
	sf::Vector2f m_Velocity;
	bool  m_Direction,m_ToDestroy;
};

