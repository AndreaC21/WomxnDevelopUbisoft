#pragma once

#include "Displayable.h"
#include "Obstacle.h"

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
	bool m_blockLeftRight;

private:

	sf::Texture m_GhostTexture;
	sf::Vector2f m_Velocity;
	bool m_onGround;
	bool m_GhostMode;
	
};