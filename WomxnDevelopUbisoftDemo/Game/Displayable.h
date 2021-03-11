#pragma once


class Displayable : public sf::Drawable, public BoxCollideable
{
public:
	Displayable();
	Displayable(const Displayable&);

	Displayable(sf::Vector2f position, std::string pathTexture, bool textureRepeat=false);
	Displayable(sf::Vector2f position, std::string pathTexture, int width, float scale);
	Displayable(sf::Vector2f position, std::string pathTexture, int width, int height, float scale);
	Displayable(sf::Vector2f position, std::string pathTexture, float scale);

	void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	void StartEndGame() ;

	sf::Vector2f getPosition() const;

protected:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;

	sf::Vector2f m_Position;
	float m_Sprite_Scale = 0.3f;
	float m_rotation = 0;
	
	bool m_IsPlayingEndGame;
	
};