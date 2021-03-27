#include <stdafx.h>
#include <Game/Platform.h>

using namespace sf;

float Platform::scale = 0.3f;
Platform::Platform()
{

}
Platform::Platform(const Platform& p) : Displayable(p)
{
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
    m_Sprite.rotate(m_rotation);
    pos = p.pos;
}
Platform::Platform(sf::Vector2f position) : Displayable(position, "Platform\\2.png",true)
{
    m_Sprite.setScale(0.3f, 0.3f);

}

Platform::Platform(int i, int j, int w, int h, float r) : Displayable(sf::Vector2f{(float)i*w,(float)j*h}, "Platform\\2.png", w,scale)
{
    m_rotation = r;
    
    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));

   // m_Sprite.setOrigin(size * 0.5f);
    m_Sprite.rotate(r);
   // SetBoundingBox(i*w,j*h,w,h*Platform::scale/2);
    SetBoundingBox(m_Sprite.getGlobalBounds());
    pos = Platform::Position::TOP;
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

void Platform::initPosition()
{
    if (m_rotation == 0.0f)
    {
        pos = Platform::Position::TOP;
    } 
    else if (m_rotation == 90.0f)
    {
        pos = Platform::Position::LEFT;
    }
    else if (m_rotation == -90.0f)
    {
        pos = Platform::Position::RIGHT;
    }
    else
    {
        pos = Platform::Position::BOTTOM;
    }

}
Platform::Position Platform::getPos() const
{
    return this->pos;
}