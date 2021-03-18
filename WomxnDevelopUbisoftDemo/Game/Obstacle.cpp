#include "stdafx.h"
#include "Obstacle.h"

Obstacle::Obstacle()
{
    m_traversable = false;
}
Obstacle::Obstacle(const Obstacle& o) :Displayable(o)
{
    m_Sprite.setScale(0.5f, 0.5f);
    m_Sprite.rotate(m_rotation);
    m_traversable = o.m_traversable;
}
Obstacle::Obstacle(int i, int j, int w, int h,bool b) : Displayable(sf::Vector2f{ (float)i * w,(float)j * h }, "Obstacle\\Crate.png",w/13, h, 0.5f)
{
   // const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));
    m_traversable = b;

    // m_Sprite.setOrigin(size * 0.5f);
    
    // SetBoundingBox(i*w,j*h,w,h*Platform::scale/2);
    SetBoundingBox(m_Sprite.getGlobalBounds());
}
void Obstacle::Update(float deltaTime)
{

    //m_Position += m_Velocity * deltaTime;
    //m_Sprite.setPosition(m_Position);
    //SetCenter(m_Position);
}
void Obstacle::StartEndGame()
{
    m_IsPlayingEndGame = true;

}

bool Obstacle::canGhostTraverse()
{
    return this->m_traversable;
}
