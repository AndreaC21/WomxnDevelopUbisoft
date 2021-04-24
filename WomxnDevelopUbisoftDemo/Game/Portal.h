#pragma once
#include "Displayable.h"

class Portal : public Displayable
{
public:

	Portal();
	Portal(const Portal&);
	Portal(int i, int j, int case_size_x, int case_size_y);
	
};

