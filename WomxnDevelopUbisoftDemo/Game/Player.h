#pragma once

#include "Displayable.h"

class Player : public Displayable
{
public:
	Player();
	Player(const Player&);

	virtual void Update(float deltaTime);
	virtual void StartEndGame();

	void setGrounded(bool d);
	bool isGrounded() const;
	void AdjustPosition(Displayable d);

private:
	sf::Vector2f m_Velocity;
	bool m_onGround;
	
};