#pragma once
#include "Displayable.h"

class Platform : public Displayable
{
	 
public:

	enum class ePosition 
	{ 
		TOP=0, 
		BOTTOM=1, 
		RIGHT=2, 
		LEFT=3
	};

	Platform();
	Platform(const Platform&);
	Platform(int i, int j, int w,int h,float r=0);

	ePosition getPos() const;
	
private:

	ePosition pos;
	void initPosition();

	
};



