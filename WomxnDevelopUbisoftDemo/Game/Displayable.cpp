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
	m_rotation = d.m_rotation;

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

Displayable::Displayable(sf::Vector2f position, std::string pathTexture, int width,float scale)
{
	m_Position = position;
	
	if (m_Texture.loadFromFile(".\\Assets\\" + pathTexture) == false)
	{

	}
	m_Texture.setRepeated(true);
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setScale(scale, scale);
	m_Sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(width / scale), m_Texture.getSize().x));
	
	m_Sprite.setPosition(m_Position.x, m_Position.y);
	

	m_IsPlayingEndGame = false;
	
}
Displayable::Displayable(sf::Vector2f position, std::string pathTexture, int width, int height,float scale)
{
	m_Position = position;

	if (m_Texture.loadFromFile(".\\Assets\\" + pathTexture) == false)
	{

	}
	m_Texture.setRepeated(true);
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setScale(scale, scale);
	m_Sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(width / scale), static_cast<int>(height / scale)));

	m_Sprite.setPosition(m_Position.x, m_Position.y);


	m_IsPlayingEndGame = false;

}
Displayable::Displayable(sf::Vector2f position, std::string pathTexture,float scale)
{
	m_Position = position;

	m_Texture.loadFromFile(".\\Assets\\" + pathTexture);
	
	m_Sprite.setTexture(m_Texture);
	m_Sprite.setScale(scale,scale);

	m_Sprite.setPosition(m_Position.x, m_Position.y);


	m_IsPlayingEndGame = false;

}
void Displayable::Update(float deltaTime)
{
}

void Displayable::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// DrawBounding box
	

	sf::RectangleShape m_Rectangle;
	m_Rectangle.setPosition(m_BoundingBox.left, m_BoundingBox.top);
	m_Rectangle.setSize(sf::Vector2f(m_BoundingBox.width, m_BoundingBox.height));
	m_Rectangle.setOutlineThickness(3);
	m_Rectangle.setOutlineColor(sf::Color{ static_cast<uint8_t>(1 * 255.0f), static_cast<uint8_t>(0 * 255.0f), static_cast<uint8_t>(0 * 255.0f) });
	target.draw(m_Rectangle);
	

	target.draw(m_Sprite);
	
}


void Displayable::StartEndGame()
{

}
sf::Vector2f Displayable::getPosition() const
{
	return this->m_Position;
}