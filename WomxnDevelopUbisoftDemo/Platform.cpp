#include <stdafx.h>
#include <Game/Platform.h>

using namespace sf;

Platform::Platform()
{

}
Platform::Platform(const Platform& p) : Displayable(p)
{
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
    pos = p.pos;
}

Platform::Platform(int i, int j, int w, int h, float r) : Displayable("Platform\\2.png")
{
    m_Texture.setRepeated(true);
    m_Sprite.setTexture(m_Texture);
    m_Sprite_Scale = 0.3f;
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);

    m_Position = sf::Vector2f{ (float)i * w,(float)j * h };

    if (r == 0.0f || r == 180.0f)
    {
        m_Sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(w / m_Sprite_Scale), m_Texture.getSize().x));
    }
    else  if (r == 90.0f || r == -90.0f)
    {
        m_Sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(h / m_Sprite_Scale),m_Texture.getSize().x));
    }
    
    m_Sprite.setPosition(m_Position.x, m_Position.y);
    m_rotation = r;
 
   // m_Sprite.setOrigin(size * 0.5f);
    m_Sprite.rotate(r);
    initPosition();
    SetBoundingBox(m_Sprite.getGlobalBounds());
    
    //pos = Platform::Position::TOP;
}
void Platform::initPosition()
{
    if (m_rotation == 0.0f)
    {
        pos = Platform::ePosition::TOP;
    } 
    else if (m_rotation == 90.0f)
    {
        pos = Platform::ePosition::LEFT;
    }
    else if (m_rotation == -90.0f)
    {
        pos = Platform::ePosition::RIGHT;
    }
    else
    {
        pos = Platform::ePosition::BOTTOM;
    }

}
Platform::ePosition Platform::getPos() const
{
    return this->pos;
}