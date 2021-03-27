#pragma once
#include "Displayable.h"

class Platform : public Displayable
{
	 
public:
	enum class Position { TOP=0, BOTTOM=1, RIGHT=2, LEFT=3 };
	Platform();
	Platform(const Platform&);
	Platform(sf::Vector2f);
	//Platform(sf::Vector2f, int w,int r);
	Platform(int i, int j, int w,int h,float r=0);

	static float scale;

	virtual void Update(float deltaTime);
	virtual void StartEndGame();

	Position getPos() const;

	const char* affiche() {
		switch (pos)
		{
		case Platform::Position::TOP: return "top";
			
		case Platform::Position::BOTTOM: return "b";
			
		case Platform::Position::RIGHT: return "r";
			
		case Platform::Position::LEFT: return "l";
			
		default: return "";
		
		}
		
	}
private:

	Position pos;

	void initPosition();

	
};



