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

	bool isGhostMode();
	void Switch();
	virtual int AdjustPosition(Displayable* d) { return 0; }
	void setCollision(int collision);

	void UpdateCollisionWithDisplayable(std::vector<Displayable*>);
	void UpdateWeaponCollisionWith(std::vector<Ennemy>& listEnnemy);

	Player* getCurrentState();
	bool* m_BlockDirection;
	sf::Vector2f m_Velocity;
	
protected:
	float SPEED_MAX;
	

	void FlipSprite(bool);

private:

	Player* m_CurrentState;
	
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
	
	virtual int AdjustPosition(Displayable* d);
	void CheckWeaponCollisionWithDisplayable(Displayable*);
	void CheckWeaponCollisionWithEnnemy(Ennemy&);

	bool isGrounded() const;
	bool isDead() const;
	std::string getLifePoint() const;
	float getCurrentLifePoint() const;
	void loseLifePoint(float amount);
	std::vector<Weapon> getWeapons();


private:
	const float JUMP_MAX = 600.0f;
	bool m_IsDead;
	float m_lifePoint_max, m_lifePoint, m_attack, m_TimePreviousShoot, m_DurationShoot;
	std::vector<Weapon> m_listWeapon;
	int m_currentThrowableWeapon, m_maxThrowableWeapon;
	sf::Clock m_clock;

	bool isShootAvailable() const;

};