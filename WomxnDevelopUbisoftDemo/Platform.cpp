#include <stdafx.h>
#include <Game/Platform.h>

using namespace sf;
Platform::Platform()
{

}
Platform::Platform(const Platform& p) : Displayable(p)
{
    m_Sprite.setScale(0.3f, 0.3f);
}
Platform::Platform(sf::Vector2f position) : Displayable(position, "Platform\\2.png",true)
{
    m_Sprite.setScale(0.3f, 0.3f);

}
Platform::Platform(sf::Vector2f position, int w,int r) : Displayable(position, "Platform\\2.png",w)
{
    m_Sprite.setScale(0.3f, 0.3f);
    m_Sprite.rotate(r);
    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));

    m_Sprite.setOrigin(size * 0.5f);
    //m_Sprite.rotate(90);
    SetBoundingBox(m_Sprite.getGlobalBounds());
    
}
void Platform::Update(float deltaTime)
{

    //m_Position += m_Velocity * deltaTime;
    //m_Sprite.setPosition(m_Position);
    //SetCenter(m_Position);
}
void Platform::StartEndGame()
{
    m_IsPlayingEndGame = true;

}