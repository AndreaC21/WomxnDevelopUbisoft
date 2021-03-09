#include <stdafx.h>
#include <Game/Platform.h>

using namespace sf;

float Platform::scale = 0.3f;
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

Platform::Platform(int i, int j, int w, int h) : Displayable(sf::Vector2f{(float)i*w,(float)j*h}, "Platform\\2.png", w,scale)
{
    
    
    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));

   // m_Sprite.setOrigin(size * 0.5f);
    //m_Sprite.rotate(90);
    SetBoundingBox(i*w,j*h,w,h*Platform::scale/2);
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