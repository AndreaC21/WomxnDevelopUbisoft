#include "stdafx.h"
#include "Portal.h"
#include <Engine/GameSpriteLoader.h>

Portal::Portal(int x, int y, int case_size_x, int case_size_y) : Displayable(sf::Vector2f(static_cast<float>(x* case_size_x + (case_size_x / 2)), static_cast<float>(y * case_size_y + (case_size_y / 2))))
{
    m_Sprite.setTexture(GameSpriteLoader::TexturePortal);
    m_Sprite_Scale = 3.0f;
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);

    SetBoundingBox(m_Sprite.getGlobalBounds());
}

Portal::Portal(const Portal& p) : Displayable(p)
{
    m_Sprite_Scale = p.m_Sprite_Scale;
    m_Sprite.setTexture(GameSpriteLoader::TexturePortal);
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
}

