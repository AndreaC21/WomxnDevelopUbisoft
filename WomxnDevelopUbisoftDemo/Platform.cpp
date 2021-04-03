#include <stdafx.h>
#include <Game/Platform.h>

using namespace sf;

float Platform::scale = 0.3f;

Platform::Platform()
{

}
Platform::Platform(const Platform& p) : Displayable(p)
{
    m_Sprite.setScale(scale, scale);
    pos = p.pos;
}

Platform::Platform(int i, int j, int w, int h, float r) : Displayable("Platform\\2.png")
{
    m_Texture.setRepeated(true);
    m_Sprite.setTexture(m_Texture);
    m_Sprite.setScale(scale, scale);

    m_Position = sf::Vector2f{ (float)i * w,(float)j * h };

    if (r == 0.0f || r == 180.0f)
    {
        m_Sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(w / scale), m_Texture.getSize().x));
    }
    else  if (r == 90.0f || r == -90.0f)
    {
        m_Sprite.setTextureRect(sf::IntRect(0, 0, static_cast<int>(h / scale),m_Texture.getSize().x));
    }
    
    m_Sprite.setPosition(m_Position.x, m_Position.y);
    m_rotation = r;
 
   // m_Sprite.setOrigin(size * 0.5f);
    m_Sprite.rotate(r);
    initPosition();
    SetBoundingBox(m_Sprite.getGlobalBounds());
    
    //pos = Platform::Position::TOP;
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