#pragma once
#include "Displayable.h"

class Obstacle : public Displayable
{
public:
	Obstacle();
	Obstacle(const Obstacle&);

	Obstacle(int i, int j, int w, int h,bool b=false);

	bool canGhostTraverse();
private:
	bool m_traversable;
};

