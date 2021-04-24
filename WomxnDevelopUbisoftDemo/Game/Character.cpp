#include "stdafx.h"
#include "Character.h"

Character::Character()
{

}
Character::Character(const Character& c) : Displayable(c)
{
    m_IsDead = c.m_IsDead;
    m_Collision = c.m_Collision;
    m_Velocity = c.m_Velocity;
    m_SpeedMax = c.m_SpeedMax;
    m_MaxLifePoint = c.m_MaxLifePoint;
    m_CurrentLifePoint = c.m_CurrentLifePoint;
    m_Attack = c.m_Attack;
    m_Clock = c.m_Clock;
    m_ToDestroy = c.m_ToDestroy;

}
Character::Character(sf::Vector2f position, std::string pathTexture) : Displayable(position,pathTexture)
{
    this->m_Collision = new bool[4]{ false,false,false,false };

    m_ToDestroy = true;
    m_IsDead = true;
    m_Velocity = sf::Vector2f(0.0f,0.0f);
    m_SpeedMax = 0;
    m_CurrentLifePoint = 0.0f;
    m_MaxLifePoint = 0.0f;
    m_Attack = 0.0f;
}


#pragma region SETTER
void Character::SetCollision(eDirection collision)
{
    this->m_Collision[collision] = true;

    if (collision == eDirection::Top || collision == eDirection::Bottom)
    {
        m_Velocity.y = 0;
    }
    else if (collision == eDirection::Left || collision == eDirection::Right)
    {
        m_Velocity.x = 0;
    }
}
void Character::ResetCollision()
{
    this->m_Collision[eDirection::Top] = false;
    this->m_Collision[eDirection::Bottom] = false;
    this->m_Collision[eDirection::Left] = false;
    this->m_Collision[eDirection::Right] = false;
}
#pragma endregion

#pragma region GETTER


std::string Character::GetLifePoint() const
{
    return std::to_string(m_CurrentLifePoint) + "/" + std::to_string(m_MaxLifePoint);
}
float Character::GetCurrentLifePoint() const
{
    return this->m_CurrentLifePoint;
}
void Character::LoseLifePoint(float amount)
{
    this->m_CurrentLifePoint -= amount;
}
bool Character::IsGrounded() const
{
    return this->m_Collision[eDirection::Bottom] == true;
}
bool Character::IsDead() const
{
    return this->m_CurrentLifePoint <= 0.0f;
}
bool Character::ToDestroy() const
{
    return m_ToDestroy;
}
#pragma endregion

void Character::FlipSprite(bool direction)
{
    if (direction) // true->right
    {
        m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
    }
    else
    {
        m_Sprite.setScale(-m_Sprite_Scale, m_Sprite_Scale);
    }
}


