#include <stdafx.h>
#include <Game/Player.h>
#include <Game/Portal.h>

using namespace sf;

#pragma region Player
Player::Player(const Player& p) : Displayable(p)
{
    m_Velocity = p.m_Velocity;
    m_Sprite.setScale(0.3f, 0.3f);
    m_BlockDirection = p.m_BlockDirection;
    m_Position = p.m_Position;
   
    SetBoundingBox(m_Sprite.getGlobalBounds());
}
Player::Player() : Displayable(sf::Vector2f(100, 100), "Player\\Idle_1.png")
{
    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));

    //Explorator
    m_ExploratorSpeedMax = 300.0f;
    m_lifePoint_max = 100.0f;
    m_lifePoint = m_lifePoint_max;
    m_attack = 25.0f;
    m_maxThrowableWeapon = 5;
    m_currentThrowableWeapon = 0;
    m_DurationShoot = 0.1f;
    m_TimePreviousShoot = 0.0f;
    m_IsDead = false;
    m_isGhost = false;
    m_ExploratorJumpMax = 600;
    
    //Ghost
    m_GhostSpeedMax = 400.0f;
    m_GhostDuration = 5.0f;
    m_GhostTexture.loadFromFile(".\\Assets\\Ghost\\Idle_1.png");

    // Player
    m_SpeedMax = m_ExploratorSpeedMax;

    m_Sprite_Scale = 0.3f;
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
    m_Sprite.setOrigin(size.x * 0.5f, 0);
    
    m_BlockDirection = new bool[4]{ false,false,false,false };
}

void Player::Update(float deltaTime)
{
    if (m_IsPlayingEndGame)
    {
        return;
    }

    

    if (Keyboard::isKeyPressed(Keyboard::G))
    {
        Switch();
    }

    

    if (Keyboard::isKeyPressed(Keyboard::Right) && m_BlockDirection[eDirection::Right] == false)
    {
        m_Velocity.x = m_SpeedMax;
        FlipSprite(true);
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left) && m_BlockDirection[eDirection::Left] == false)
    {
        m_Velocity.x = -m_SpeedMax;
        FlipSprite(false);
    }
    else
    {
        m_Velocity.x *= 0;
    }

    

    if (isGhostMode() == false)
    {
        if (isGrounded() == false || m_BlockDirection[eDirection::Top] == true)
        {
            //Fall
            m_Velocity.y = fmin(m_Velocity.y + 10, m_ExploratorJumpMax);
        }
        if (Keyboard::isKeyPressed(Keyboard::Space) && isGrounded())
        {
            m_Velocity.y = -m_ExploratorJumpMax;
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isShootAvailable())
        {
            if (m_currentThrowableWeapon < m_maxThrowableWeapon)
            {
                m_TimePreviousShoot = m_clock.getElapsedTime().asSeconds();
                m_currentThrowableWeapon++;

                bool direction = m_Sprite.getScale().x > 0;
                m_listWeapon.push_back(Weapon(GetCenter(), direction, m_TimePreviousShoot));
            }
        }

        for (int i = 0; i < (int)m_listWeapon.size(); ++i)
        {
            m_listWeapon[i].Update(deltaTime);
            if (m_listWeapon[i].Finish())
            {
                m_listWeapon.erase(std::find(m_listWeapon.begin(), m_listWeapon.end(), m_listWeapon[i]));
                m_currentThrowableWeapon--;
            }
        }
    }
    else
    {
        if (Keyboard::isKeyPressed(Keyboard::Down) && m_BlockDirection[eDirection::Bottom] == false)
        {
            m_Velocity.y = m_SpeedMax;

        }
        else if (Keyboard::isKeyPressed(Keyboard::Up) && m_BlockDirection[eDirection::Top] == false)
        {
            m_Velocity.y = -m_SpeedMax;
        }
        else
        {
            m_Velocity.y *= 0;
        }
    }
   
    m_Position += m_Velocity * deltaTime;
    m_Sprite.setPosition(m_Position);
    SetBoundingBox(m_Sprite.getGlobalBounds());
   
}
void Player::UpdateCollisionWithDisplayable(std::vector<Displayable*> listDisplayable)
{
    //ResetCollision
    this->m_BlockDirection[0] = false;
    this->m_BlockDirection[1] = false;
    this->m_BlockDirection[2] = false;
    this->m_BlockDirection[3] = false;
    for (int i = 0; i < listDisplayable.size(); ++i)
    {
        if (this->IsColliding(*listDisplayable[i]))
        {
            this->AdjustPosition(listDisplayable[i]);
        }

        if (isGhostMode() == false)
        {
            this->CheckWeaponCollisionWithDisplayable(listDisplayable[i]);
        }
    }
}
void Player::UpdateWeaponCollisionWith(std::vector<Ennemy>& listEnnemy)
{
    if (isGhostMode() == true)
    {
        return;
    }
    for (int i = 0; i < listEnnemy.size(); ++i)
    {
        this->CheckWeaponCollisionWithEnnemy(listEnnemy[i]);
    }
}


void Player::setCollision(int collision)
{
    m_BlockDirection[collision] = true;
    
    if (collision == eDirection::Top || collision == eDirection::Bottom)
    {
        m_Velocity.y = 0;
     
    }
    else if (collision == eDirection::Left || collision == eDirection::Right)
    {
        m_Velocity.x = 0;
    
    }
}

void Player::Switch()
{
    if (isGhostMode())
    {
        m_isGhost = false;
        m_Sprite.setTexture(m_Texture);
        m_SpeedMax = m_ExploratorSpeedMax;
    }
    else
    {
        m_isGhost = true;
        m_Sprite.setTexture(m_GhostTexture);
        m_SpeedMax = m_GhostSpeedMax;
    }
    SetBoundingBox(m_Sprite.getGlobalBounds());
   
}

void Player::StartEndGame()
{
	m_IsPlayingEndGame = true;

}

bool Player::isGhostMode()
{
    return  m_isGhost;
}

void Player::FlipSprite(bool directionRight)
{
    if (directionRight)
    {
        m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
    }
    else
    {
        m_Sprite.setScale(-m_Sprite_Scale, m_Sprite_Scale);
    }
}

int Player::AdjustPosition(Displayable* d)
{
    if (isGhostMode())
    {
        if (typeid(*d) == typeid(Obstacle))
        {
            Obstacle* o = dynamic_cast<Obstacle*>(d);
            if (o != nullptr && o->canGhostTraverse())
            {
                return -1;
            }
        }
        else if (typeid(*d) == typeid(Portal))
        {
            return -1;
        }
    }
    int index_collision = this->CollisionDirection(*d);
    if (index_collision != -1)
    {
        setCollision(index_collision);
    }
    return index_collision;
}
bool Player::isShootAvailable() const
{
    if (m_clock.getElapsedTime().asSeconds() > this->m_TimePreviousShoot + this->m_DurationShoot)
    {
        return true;
    }
    return false;
}
void Player::CheckWeaponCollisionWithDisplayable(Displayable* d)
{
    for (int j = 0; j < m_listWeapon.size(); ++j)
    {
        if (m_listWeapon[j].IsColliding(*d))
        {
            m_listWeapon[j].TouchDisplayable(d);
        }
    }
}
void Player::CheckWeaponCollisionWithEnnemy(Ennemy& e)
{
    for (int j = 0; j < m_listWeapon.size(); ++j)
    {
        if (m_listWeapon[j].IsColliding(e))
        {
            m_listWeapon[j].TouchEnnemy(e);
        }
    }
}
std::string Player::getLifePoint() const
{
    return std::to_string(m_lifePoint) + "/" + std::to_string(m_lifePoint_max);
}
float Player::getCurrentLifePoint() const
{
    return this->m_lifePoint;
}
void Player::loseLifePoint(float amount)
{
    this->m_lifePoint -= amount;
}

std::vector<Weapon> Player::getWeapons()
{
    return m_listWeapon;
}
bool Player::isGrounded() const
{
    return this->m_BlockDirection[eDirection::Bottom] == true;
}
bool Player::isDead() const
{
    return this->m_lifePoint <= 0.0f;
}

bool Player::getBlockDirection()
{
    return m_BlockDirection[0];
        /*" - bottom : " + std::to_string(m_BlockDirection[1]) +
        " - left : " + std::to_string(m_BlockDirection[2]) +
        "- right : " + std::to_string(m_BlockDirection[3])).c_str();*/
}