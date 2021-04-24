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

    m_traversable = b;

    SetBoundingBox(m_Sprite.getGlobalBounds());
}

bool Obstacle::canGhostTraverse()
{
    return this->m_traversable;
}
