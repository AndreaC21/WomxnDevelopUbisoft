#pragma once

#include "Displayable.h"
#include "Obstacle.h"
#include "Weapon.h"
#include "Ennemy.h"


class Player : public Displayable
{
public:
	Player();
	Player(const Player&);

	void Init(std::vector<Ennemy>& m_ptr_listEnnemy, const std::vector<Displayable*>&);
	virtual void Update(float deltaTime);
	virtual void StartEndGame();

	bool isGhostMode();

	void UpdateCollisionWithDisplayable(std::vector<Displayable*>);
	void UpdateWeaponCollisionWithEnnemy();

	bool isGrounded() const;
	bool isDead() const;
	std::string getLifePoint() const;
	float getCurrentLifePoint() const;
	void loseLifePoint(float amount);
	std::vector<Weapon> getWeapons();
	void CheckWeaponCollisionWithDisplayable(Displayable*);
	void CheckWeaponCollisionWithEnnemy(Ennemy&);

	bool getBlockDirection();

private:

	bool* m_BlockDirection;
	sf::Vector2f m_Velocity;
	float m_SpeedMax;
	std::vector<Ennemy> m_ptr_listEnnemy;
	std::vector<Displayable*> m_ptr_listDisplayable;

	//Explorator
	bool m_IsDead;
	float m_lifePoint_max, m_lifePoint, m_attack, m_TimePreviousShoot, m_DurationShoot;
	std::vector<Weapon> m_listWeapon;
	int m_currentThrowableWeapon, m_maxThrowableWeapon;
	sf::Clock m_clock;
	sf::Vector2f m_ExploratorDeadPosition;
	float m_ExploratorSpeedMax;
	float m_ExploratorJumpMax;

	//Ghost
	sf::Texture m_GhostTexture;
	bool m_isGhost;
	float m_GhostDuration;
	float m_GhostSpeedMax;

	bool isShootAvailable() const;
	void FlipSprite(bool);
	void Switch();
	int AdjustPosition(Displayable* d);
	void setCollision(int collision);
	void UpdateCollision();
	void UpdateGhostAction();
	void UpdateExploratorAction(float deltaTime);
	
};	
//600.0f;
