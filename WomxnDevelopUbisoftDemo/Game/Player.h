#pragma once

#include "Displayable.h"
#include "Obstacle.h"
#include "Weapon.h"

class Player : public Displayable
{
public:
	Player();
	Player(const Player&);

	virtual void Update(float deltaTime);
	virtual void StartEndGame();

	void setGrounded(bool d);
	bool isGrounded() const;
	bool isGhostMode();
	void AdjustPosition(Displayable* d);
	std::string getLifePoint() const;
	float getCurrentLifePoint() const;
	std::vector<Weapon> getWeapon() const;

	bool m_blockLeftRight,m_CanShoot;
	
	void UpdateShoot(sf::Time t);

private:

	sf::Texture m_GhostTexture;
	sf::Vector2f m_Velocity;
	bool m_onGround, m_GhostMode;
	float m_lifePoint_max, m_lifePoint, m_attack,m_TimePreviousShoot,m_DurationShoot;
	std::vector<Weapon> listWeapon;
	int m_currentThrowableWeapon,m_maxThrowableWeapon;

	sf::Time clock;

	void SwitchMode();
	

public:
	bool test_boolean = false;
	
};