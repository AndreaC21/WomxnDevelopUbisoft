#include <stdafx.h>
#include <Game/Player.h>
#include <typeinfo>

using namespace sf;


Player::Player(const Player& p) : Displayable(p)
{
    m_Velocity = p.m_Velocity;
    m_Sprite.setScale(0.3f, 0.3f);
    m_GhostMode = p.m_GhostMode;
    m_onGround = p.m_onGround;
    m_blockLeftRight = p.m_blockLeftRight;
    m_lifePoint = p.m_lifePoint;
    m_attack = p.m_attack;
    SetBoundingBox(m_Sprite.getGlobalBounds());
}
Player::Player() : Displayable(sf::Vector2f(100,150), "Player\\Idle_1.png")
{
    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));

    m_Sprite_Scale = 0.3f;
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
   // m_Sprite.setOrigin(size *0.5f); // middle
    m_onGround = false;
    m_blockLeftRight = false;
    m_GhostMode = false;
   
    m_lifePoint_max = 100.0f;
    m_lifePoint = m_lifePoint_max;
    m_attack = 25.0f;
    m_maxThrowableWeapon = 5;
    m_currentThrowableWeapon = 0;
    m_DurationShoot = 0.1f;
    m_TimePreviousShoot = 0.0f;
    m_CanShoot = true;
  
    //left,top
    SetBoundingBox(m_Position.x,m_Position.y,size.x*m_Sprite_Scale,size.y*m_Sprite_Scale);

    m_GhostTexture.loadFromFile(".\\Assets\\Ghost\\Idle_1.png");
   
}

void Player::Update(float deltaTime)
{
    if (m_IsPlayingEndGame)
    {
        return;
    }

    const float SPEED_MAX = 300.0f;
    const float SPEED_INC = 10.0f;
    const float DEAD_ZONE = 5.0f;
    const float SLOWDOWN_RATE = 0.9f;
    const float JUMP_MAX = 600.0f;

  
    /*if (m_IsUsingJoystick)
    {
        m_Velocity.x = GetScaledAxis(m_JoystickIndex, Joystick::Axis::X, DEAD_ZONE, SPEED_MAX);
        m_Velocity.y = GetScaledAxis(m_JoystickIndex, Joystick::Axis::Y, DEAD_ZONE, SPEED_MAX);

        if (Joystick::isButtonPressed(m_JoystickIndex, 0))
        {
            if (!m_WasButtonPressed)
            {
                m_Sprite.setScale(0.8f, 0.8f);
                m_WasButtonPressed = true;
            }
        }
        else
        {
            if (m_WasButtonPressed)
            {
                m_Sprite.setScale(1.0f, 1.0f);
                m_WasButtonPressed = false;
            }
        }
    }
    else
    {
    */
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {
        m_Velocity.x = fmin(m_Velocity.x + SPEED_INC, SPEED_MAX);
        // m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
         //m_Sprite.setOrigin(0, 0);
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left))
    {
        m_Velocity.x = fmax(m_Velocity.x - SPEED_INC, -SPEED_MAX);
        // m_Sprite.setScale(-m_Sprite_Scale, m_Sprite_Scale);
        // m_Sprite.setOrigin(0, 0);
    }
    else 
    {
        m_Velocity.x *= SLOWDOWN_RATE;
    }
    
    if (m_GhostMode == true)
    {
        if (Keyboard::isKeyPressed(Keyboard::Down))
        {
            m_Velocity.y = fmin(m_Velocity.y + SPEED_INC, SPEED_MAX);

        }
        else if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            m_Velocity.y = fmax(m_Velocity.y - SPEED_INC, -SPEED_MAX);

        }
        else
        {
            m_Velocity.y *= SLOWDOWN_RATE;
        }
    }
    else
    {
        if (Keyboard::isKeyPressed(Keyboard::Space) && m_onGround)
        {
            m_Velocity.y = -JUMP_MAX;// fmax(m_Velocity.y - SPEED_INC, -SPEED_MAX);
            m_onGround = false;
        }

        if (m_onGround == false)
        {
            m_Velocity.y = fmin(m_Velocity.y + SPEED_INC, JUMP_MAX);

        }
        else
        {
            m_Velocity.y = 0;
            //m_Velocity.y *= SLOWDOWN_RATE;
        }
    }

   
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_CanShoot && m_GhostMode ==false)
    {
        if (m_currentThrowableWeapon < m_maxThrowableWeapon)
        {
            m_TimePreviousShoot = clock.asSeconds();
            m_currentThrowableWeapon++;
            m_CanShoot = false;
            listWeapon.push_back(Weapon(GetCenter(),true, m_TimePreviousShoot));
        }
       
    }
    if (Keyboard::isKeyPressed(Keyboard::G))
    {
        SwitchMode();
    }

    for (int i = 0; i < (int)listWeapon.size(); ++i)
    {
        listWeapon[i].Update(deltaTime);
        if (listWeapon[i].Finish())
        {
            listWeapon.erase(std::find(listWeapon.begin(), listWeapon.end(), listWeapon[i]));
            m_currentThrowableWeapon--;
        }
    }

   /* for (auto w : listWeapon)
    {
        w.Update(deltaTime);
        if (w.Finish()) listWeapon.erase(std::find(listWeapon.begin(),listWeapon.end(),w));
    }*/


    m_Position += m_Velocity * deltaTime;
    m_Sprite.setPosition(m_Position);
    //SetBoundingBox(m_Sprite.getGlobalBounds());
    SetCenter(m_Position.x,m_Position.y);
}

void Player::AdjustPosition(Displayable* d)
{
    if (m_GhostMode && typeid(*d) == typeid(Obstacle))
    {
        Obstacle* o = static_cast <Obstacle*>(d);
        test_boolean = true;
        if (o->canGhostTraverse()) return;
        
    }
  
    int index_collision = this->index_collision(*d);

    switch (index_collision)
    {
        
        case 0: setGrounded(false); m_Velocity.y = 0; break; // up
        case 1: setGrounded(true); m_Velocity.y = 0; break; // down
        case 2: m_Velocity.x = 0; m_blockLeftRight = true; break;
        case 3: m_Velocity.x = 0; m_blockLeftRight = true; break;
   
    }
}
void Player::SwitchMode()
{
    //Not in ghost Mode
    if (m_GhostMode == false)
    {
        m_GhostMode = true;
        m_Sprite.setTexture(m_GhostTexture);
        const sf::Vector2f size(static_cast<float>(m_GhostTexture.getSize().x), static_cast<float>(m_GhostTexture.getSize().y));
        SetBoundingBox(m_Position.x, m_Position.y, size.x * m_Sprite_Scale, size.y * m_Sprite_Scale);
    }
    //Already in ghost mode
    else
    {
        m_GhostMode = false;
        m_Sprite.setTexture(m_Texture);
        const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));
        SetBoundingBox(m_Position.x, m_Position.y, size.x * m_Sprite_Scale, size.y * m_Sprite_Scale);
    }
   
}
void Player::UpdateShoot(sf::Time currentTime)
{
    clock = currentTime;
    if (currentTime.asSeconds() > this->m_TimePreviousShoot + this->m_DurationShoot)
    {
       m_CanShoot = true;
    }
    else
    {
        m_CanShoot = false;
    }
}

void Player::StartEndGame()
{
	m_IsPlayingEndGame = true;

}
bool Player::isGrounded() const
{
    return m_onGround;
}
void Player::setGrounded(bool b)
{
    m_onGround = b;
}
bool Player::isGhostMode()
{
    return this->m_GhostMode;
}

std::string Player::getLifePoint() const
{
    return std::to_string(m_lifePoint) + "/" + std::to_string(m_lifePoint_max);
}
float Player::getCurrentLifePoint() const
{
    return this->m_lifePoint;
}
std::vector<Weapon> Player::getWeapon() const
{
    return this->listWeapon;
}