#pragma once
#include "Displayable.h"

class Portal : public Displayable
{
public:

	Portal();
	Portal(int i, int j, int case_size_x, int case_size_y);
	Portal(const Portal&);

	virtual void Update(float deltaTime);
	virtual void StartEndGame();
};

