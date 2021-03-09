#pragma once
#include "Displayable.h"

class Platform : public Displayable
{
public:
	Platform();
	Platform(const Platform&);
	Platform(sf::Vector2f);
	//Platform(sf::Vector2f, int w,int r);
	Platform(int i, int j, int w,int h);

	static float scale;

	virtual void Update(float deltaTime);
	virtual void StartEndGame();

	
};

