#pragma once

#include "Displayable.h"
#include "Obstacle.h"
#include "Weapon.h"
#include "Ennemy.h"


class Player : public Displayable
{
public:
	Player();
	Player(sf::Vector2f, const char* s);
	Player(const Player&);

	virtual void Update(float deltaTime);
	virtual void StartEndGame();

	void NoCollisionDetected(int);
	bool isGhostMode();
	void Switch();
	virtual int AdjustPosition(Displayable* d);
	void setCollision(int  collision);

	Player* getCurrentState();
	bool* m_BlockDirection;
	
protected:

	sf::Vector2f m_Velocity;
	bool m_GhostMode;
	sf::Time clock;
	Player* m_CurrentState;

	void FlipSprite(bool);

};

class Ghost : public Player
{
public:

	Ghost(sf::Vector2f);
	Ghost(const Ghost&);

	virtual void Update(float deltaTime);
	virtual int AdjustPosition(Displayable* d);
	

private:
	float m_Duration;
};

class Explorator : public Player
{
public:

	Explorator(sf::Vector2f);
	Explorator(const Explorator&);

	virtual void Update(float deltaTime);
	void UpdateWeapon(Displayable* d);
	void UpdateWeapon(Ennemy& e);
	virtual int AdjustPosition(Displayable* d);

	std::vector<Weapon> getWeapon() const;
	void UpdateShoot(sf::Time t);
	bool WeaponTouch(int index_weapon, Displayable* d);
	bool WeaponTouch(int index_weapon, Ennemy& e);
	bool isGrounded() const;
	bool isDead() const;
	std::string getLifePoint() const;
	float getCurrentLifePoint() const;
	void loseLifePoint(float amount);


private:
	bool m_CanShoot,m_IsDead;
	float m_lifePoint_max, m_lifePoint, m_attack, m_TimePreviousShoot, m_DurationShoot;
	std::vector<Weapon> m_listWeapon;
	int m_currentThrowableWeapon, m_maxThrowableWeapon;

};