#pragma once

#include "Character.h"
#include "Obstacle.h"
#include "Weapon.h"
#include "Ennemy.h"


class Player : public Character
{
public:
	Player();

	virtual void Update(float deltaTime);
	virtual void StartEndGame();

	virtual void OnCollide(Platform&);
	virtual void OnCollide(Obstacle&);
	void OnCollide(Ennemy&);

	int GetGhostCurrentTime() const;
	int GetTimeSpendInGhost() const;
	bool IsGhostMode();
	std::vector<Weapon>& GetWeapons();

private:
	bool m_HasSwitched;
	
	//Explorator

	float m_TimePreviousShoot, m_DurationShoot;
	std::vector<Weapon> m_Weapons;
	int m_CurrentThrowableWeapon, m_MaxThrowableWeapon;
	sf::Vector2f m_ExploratorDeadPosition;
	float m_ExploratorSpeedMax;
	float m_ExploratorJumpMax;

	//Ghost
	sf::Texture m_GhostTexture;
	bool m_IsGhost;
	float m_GhostDuration;
	float m_GhostSpeedMax;
	float m_GhostEndTime;
	int m_CounterGhost;

	bool isShootAvailable() const;	
	void Switch();
	void BecomeGhost();
	void ReturnToPlayer();
	void UpdateGhostAction();
	void UpdateExploratorAction(float deltaTime);
	void SetLifePoint(float);
	
};	
