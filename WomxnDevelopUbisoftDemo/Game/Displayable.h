#pragma once


class Displayable : public sf::Drawable, public BoxCollideable
{
public:
	Displayable();
	Displayable(const Displayable&);
	Displayable(std::string pathTexture);
	Displayable(sf::Vector2f position);
	Displayable(sf::Vector2f position, std::string pathTexture, bool textureRepeat=false);
	Displayable(sf::Vector2f position, std::string pathTexture, int width, float scale);
	Displayable(sf::Vector2f position, std::string pathTexture, int width, int height, float scale);
	Displayable(sf::Vector2f position, std::string pathTexture, float scale);

	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	sf::Vector2f GetPosition() const;
	
protected:

	sf::Texture m_Texture;
	sf::Sprite m_Sprite;	
	sf::Vector2f m_Position;
	float m_Sprite_Scale = 0.3f;
	float m_rotation = 0;

	
};