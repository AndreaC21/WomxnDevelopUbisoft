#pragma once
#include "Displayable.h"
#include <Game/Obstacle.h>
#include <Game/Platform.h>
#include <Game/Ennemy.h>

class Weapon : public Character
{
public:
	Weapon();
	Weapon(const Weapon&);
	Weapon(sf::Vector2f position,bool direction, float timeStart);

	virtual void Update(float deltaTime);
	virtual void StartEndGame();
	bool ToDestroy() const;
	void OnCollide(Platform&);
	void OnCollide(Obstacle&);
	void OnCollide(Ennemy&);
	
private:
	float m_Duration,m_TimeStart,m_TimeEnd;
	
	/*bool m_ToDestroy;
	sf::Vector2f m_Velocity;
	float m_SpeedMax;
	float m_Attack;
	sf::Clock m_Clock;*/
	float m_SpeedInc = 10.0f;

public:
	bool operator==(const Weapon& w)
	{
		return (this->m_Position == w.m_Position && this->m_TimeStart == w.m_TimeStart);

	}
};

