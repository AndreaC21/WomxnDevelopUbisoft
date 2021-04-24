#pragma once
#include "Displayable.h"
class Character : public Displayable
{
public:

	Character();
	Character(const Character&);
	Character(sf::Vector2f position, std::string pathTexture);

	virtual void Update(float deltaTime)=0;
	virtual void StartEndGame()=0;
	virtual void OnCollide(Platform&) = 0;
	virtual void OnCollide(Obstacle&) = 0;
	virtual void OnCollide(Ennemy&) {};
	virtual void OnCollide(Player&) {};
	

	//SETTER
	void SetCollision(eDirection collision);

	//GETTER
	bool IsGrounded() const;
	bool IsDead() const;
	std::string GetLifePoint() const;
	float GetCurrentLifePoint() const;
	bool ToDestroy() const;
	void LoseLifePoint(float amount);
	void ResetCollision();

	void FlipSprite(bool direction);

protected:

	//bool m_Direction;
	bool m_IsDead, m_ToDestroy;
	bool* m_Collision;
	sf::Vector2f m_Velocity;
	float m_SpeedMax;
	float m_MaxLifePoint, m_CurrentLifePoint;
	float m_Attack;
	sf::Clock m_Clock;
};

