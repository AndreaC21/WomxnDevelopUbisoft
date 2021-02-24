#pragma once

#include "Displayable.h"

class Player : public Displayable
{
public:
	Player();
	Player(const Player&);
	Player(sf::Vector2f startPosition);

	void Update(float deltaTime) override;
	void StartEndGame() override;

private:
	sf::Vector2f m_Velocity;
};