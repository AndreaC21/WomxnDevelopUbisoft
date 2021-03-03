#include <stdafx.h>
#include "Game/Displayable.h"
#include <iostream>

using namespace sf;

Displayable::Displayable() 
{
	m_IsPlayingEndGame = false;
}
Displayable::Displayable(const Displayable& d)
{
	m_Texture = d.m_Texture;
	
	m_Position = d.m_Position;
	m_BoundingBox = d.m_BoundingBox;
	m_IsPlayingEndGame = d.m_IsPlayingEndGame;

	m_Sprite = d.m_Sprite;
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setPosition(m_Position);
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

Displayable::Displayable(sf::Vector2f position, std::string pathTexture, int width)
{
	m_Position = position;
	
	if (m_Texture.loadFromFile(".\\Assets\\" + pathTexture) == false)
	{

	}
	m_Texture.setRepeated(true);
	m_Sprite.setTexture(m_Texture);
	
	m_Sprite.setTextureRect(sf::IntRect(0, 0, width, m_Texture.getSize().x));
	
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
sf::Vector2f Displayable::getPosition() const
{
	return this->m_Position;
}