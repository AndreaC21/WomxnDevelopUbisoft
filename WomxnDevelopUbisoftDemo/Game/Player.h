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

	virtual void Update(float deltaTime);
	virtual void StartEndGame();
	virtual void OnCollide(Obstacle&) override;
	virtual void OnCollide(Ennemy&) override;
	virtual void OnCollide(Platform&) override;
	virtual void OnCollide(Displayable*&) override;

	bool IsGhostMode();

	bool IsGrounded() const;
	bool IsDead() const;
	std::string GetLifePoint() const;
	float GetCurrentLifePoint() const;
	void LoseLifePoint(float amount);
	std::vector<Weapon>& GetWeapons();

	void ResetBlockCollision();

private:

	bool* m_BlockDirection;
	sf::Vector2f m_Velocity;
	float m_SpeedMax;
	bool m_HasSwitched;
	
	//Explorator
	bool m_IsDead;
	float m_LifePointMax, m_lifePoint, m_Attack, m_TimePreviousShoot, m_DurationShoot;
	std::vector<Weapon> m_Weapons;
	int m_CurrentThrowableWeapon, m_MaxThrowableWeapon;
	sf::Clock m_Clock;
	sf::Vector2f m_ExploratorDeadPosition;
	float m_ExploratorSpeedMax;
	float m_ExploratorJumpMax;

	//Ghost
	sf::Texture m_GhostTexture;
	bool m_IsGhost;
	float m_GhostDuration;
	float m_GhostSpeedMax;

	bool isShootAvailable() const;
	void FlipSprite(bool);
	void Switch();
	void SetCollision(int collision);

	void UpdateGhostAction();
	void UpdateExploratorAction(float deltaTime);
	
};	
//600.0f;
