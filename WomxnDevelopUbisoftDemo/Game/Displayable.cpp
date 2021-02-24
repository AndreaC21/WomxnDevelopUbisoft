#include <stdafx.h>
#include "Game/Displayable.h"
#include <iostream>

using namespace sf;

Displayable::Displayable() 
{
	m_IsPlayingEndGame = false;
}

Displayable::Displayable(sf::Vector2f position, std::string pathTexture, bool textureRepeat )
{
	m_Position = position;


	if (m_Texture.loadFromFile(".\\Assets\\" + pathTexture)==false)
	{
		
	}
	m_Texture.setRepeated(textureRepeat);
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setPosition(m_Position.x, m_Position.y);

	m_IsPlayingEndGame = false;
}

void Displayable::Update(float deltaTime)
{
}

void Displayable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_Sprite);
}

void Displayable::StartEndGame()
{
}
