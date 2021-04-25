#include <stdafx.h>
#include <Game/Player.h>
#include <Game/Portal.h>

using namespace sf;

#pragma region Player

Player::Player() : Character(sf::Vector2f(100, 100), "Player\\Idle_1.png")
{
    //Explorator
    m_ExploratorSpeedMax = 300.0f;
    m_MaxLifePoint = 100.0f;
    m_CurrentLifePoint = m_MaxLifePoint;
    m_MaxThrowableWeapon = 5;
    m_CurrentThrowableWeapon = 0;
    m_DurationShoot = 0.1f;
    m_TimePreviousShoot = 0.0f;
    m_IsDead = false;
    m_IsGhost = false;
    m_ExploratorJumpMax = 600;
    
    //Ghost
    m_GhostSpeedMax = 400.0f;
    m_GhostDuration = 5.0f;
    m_GhostTexture.loadFromFile(".\\Assets\\Ghost\\Idle_1.png");

    // Player
    m_SpeedMax = m_ExploratorSpeedMax;

    m_Sprite_Scale = 0.3f;
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));
    m_Sprite.setOrigin(size.x * 0.5f, 0);
    
    m_HasSwitched = false;

}

void Player::Update(float deltaTime)
{
    if (m_IsPlayingEndGame)
    {
        return;
    }

    if (Keyboard::isKeyPressed(Keyboard::G) && m_HasSwitched==false)
    {
        Switch();
        m_HasSwitched = true;
    }
    if (!Keyboard::isKeyPressed(Keyboard::G) && m_HasSwitched == true)
    {
        m_HasSwitched = false;
    }

    if (Keyboard::isKeyPressed(Keyboard::Right) && m_Collision[eDirection::Right] == false)
    {
        m_Velocity.x = m_SpeedMax;
        FlipSprite(true);
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left) && m_Collision[eDirection::Left] == false)
    {
        m_Velocity.x = -m_SpeedMax;
        FlipSprite(false);
    }
    else
    {
        m_Velocity.x *= 0;
    }

    if (IsGhostMode() == false)
    {
        UpdateExploratorAction(deltaTime);
    }
    else
    {
        UpdateGhostAction();
    }
   
    m_Position += m_Velocity * deltaTime;
    m_Sprite.setPosition(m_Position);

    SetBoundingBox(m_Sprite.getGlobalBounds());
}

void Player::UpdateGhostAction()
{
    if (Keyboard::isKeyPressed(Keyboard::Down) && m_Collision[eDirection::Bottom] == false)
    {
        m_Velocity.y = m_SpeedMax;
    }
    else if (Keyboard::isKeyPressed(Keyboard::Up) && m_Collision[eDirection::Top] == false)
    {
        m_Velocity.y = -m_SpeedMax;
    }
    else
    {
        m_Velocity.y *= 0;
    }

    if (m_GhostEndTime <= m_Clock.getElapsedTime().asSeconds())
    {
        m_CounterGhost++;
        ReturnToPlayer();
        Switch();
    }
    
}
void Player::UpdateExploratorAction(float deltaTime)
{
    if (IsGrounded() == false || m_Collision[eDirection::Top] == true)
    {
        //Fall
        m_Velocity.y = fmin(m_Velocity.y + 10, m_ExploratorJumpMax);
    }
    if (Keyboard::isKeyPressed(Keyboard::Space) && IsGrounded())
    {
        m_Velocity.y = -m_ExploratorJumpMax;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isShootAvailable())
    {
        if (m_CurrentThrowableWeapon < m_MaxThrowableWeapon)
        {
            m_TimePreviousShoot = m_Clock.getElapsedTime().asSeconds();
            m_CurrentThrowableWeapon++;

            bool direction = m_Sprite.getScale().x > 0;
            m_Weapons.push_back(Weapon(GetCenter(), direction, m_TimePreviousShoot));
        }
    }

    for (int i = 0; i < (int)m_Weapons.size(); ++i)
    {
        m_Weapons[i].Update(deltaTime);
        if (m_Weapons[i].ToDestroy())
        {
            m_Weapons.erase(std::find(m_Weapons.begin(), m_Weapons.end(), m_Weapons[i]));
            m_CurrentThrowableWeapon--;
        }
    }

    if (IsDead())
    {
        BecomeGhost();
        Switch();
    }
}

void Player::Switch()
{
    if (IsGhostMode())
    {
        m_IsGhost = false;
        m_Sprite.setTexture(m_Texture);
        m_SpeedMax = m_ExploratorSpeedMax;
    }
    else
    {
        m_IsGhost = true;
        m_Sprite.setTexture(m_GhostTexture);
        m_SpeedMax = m_GhostSpeedMax;
    }
    SetBoundingBox(m_Sprite.getGlobalBounds());  
}
void Player::BecomeGhost()
{
    m_ExploratorDeadPosition = getPosition();
    m_GhostEndTime = m_Clock.getElapsedTime().asSeconds() + m_GhostDuration;
}
void Player::ReturnToPlayer()
{
    if ( getPosition() == m_ExploratorDeadPosition)
    {
        SetLifePoint(m_MaxLifePoint);
    }
    else
    {
        SetLifePoint(m_MaxLifePoint * 0.5f);
    }
    m_Position = m_ExploratorDeadPosition;
}
void Player::SetLifePoint(float lifePoint)
{
    m_CurrentLifePoint = lifePoint;
}
void Player::StartEndGame()
{
	m_IsPlayingEndGame = true;
}

bool Player::IsGhostMode()
{
    return  m_IsGhost;
}


void Player::OnCollide(Obstacle& o)
{
    if (IsGhostMode() == false)
    {
        eDirection collisionToBlock = this->CollisionDirection(o);
        if (collisionToBlock != eDirection::Null)
        {
            SetCollision(collisionToBlock);
        }
    }
}
void Player::OnCollide(Ennemy&)
{
    if (IsGhostMode() == false)
    {
        // lose life point
    }
}
void Player::OnCollide(Platform& p)
{
   eDirection collisionToBlock = this->CollisionDirection(p);
    if (collisionToBlock != -1)
    {
        SetCollision(collisionToBlock);
    }
}

bool Player::isShootAvailable() const
{
    if (m_Clock.getElapsedTime().asSeconds() > this->m_TimePreviousShoot + this->m_DurationShoot)
    {
        return true;
    }
    return false;
}
int Player::GetGhostCurrentTime() const
{
    return static_cast<int>(m_GhostEndTime - m_Clock.getElapsedTime().asSeconds());
}
int Player::GetTimeSpendInGhost() const
{
    return static_cast<int>(m_CounterGhost * m_GhostDuration);
}
std::vector<Weapon>& Player::GetWeapons()
{
    return m_Weapons;
}
