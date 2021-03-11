#pragma once
#include "Displayable.h"

class Obstacle : public Displayable
{
public:
	Obstacle();
	Obstacle(const Obstacle&);

	Obstacle(int i, int j, int w, int h,bool b=false);

	//static float scale;

	virtual void Update(float deltaTime);
	virtual void StartEndGame();

	bool canGhostTraverse();
private:
	bool m_traversable;
};

