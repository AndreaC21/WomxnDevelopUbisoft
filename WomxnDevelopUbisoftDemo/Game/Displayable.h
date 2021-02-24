#pragma once


class Displayable : public sf::Drawable, public BoxCollideable
{
public:
	Displayable();

	Displayable(sf::Vector2f position, std::string pathTexture, bool textureRepeat=false);

	virtual void Update(float deltaTime);
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	virtual void StartEndGame();

protected:
	sf::Texture m_Texture;
	sf::Sprite m_Sprite;

	sf::Vector2f m_Position;
	
	bool m_IsPlayingEndGame;
};