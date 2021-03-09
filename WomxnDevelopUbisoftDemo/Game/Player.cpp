#include <stdafx.h>
#include <Game/Player.h>

using namespace sf;


Player::Player(const Player& p) : Displayable(p)
{
    m_Velocity = p.m_Velocity;
    m_Sprite.setScale(0.3f, 0.3f);
    
    m_onGround = p.m_onGround;
    SetBoundingBox(m_Sprite.getGlobalBounds());
}
Player::Player() : Displayable(sf::Vector2f(500,150), "Player\\Idle_1.png")
{
    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));

    m_Sprite_Scale = 0.3f;
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
   // m_Sprite.setOrigin(size *0.5f); // middle
    m_onGround = false;
  
    //left,top
    SetBoundingBox(m_Position.x,m_Position.y,size.x*m_Sprite_Scale,size.y*m_Sprite_Scale);
}


void Player::Update(float deltaTime)
{
    if (m_IsPlayingEndGame)
    {
        return;
    }

    const float SPEED_MAX = 150.0f;
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
        if (Keyboard::isKeyPressed(Keyboard::Space) && m_onGround )
        {
            m_Velocity.y = -JUMP_MAX;// fmax(m_Velocity.y - SPEED_INC, -SPEED_MAX);
            m_onGround = false;
        }
        
        /*
        if (m_onGround==false)
        {
            m_Velocity.y = fmin(m_Velocity.y + SPEED_INC, JUMP_MAX);
            
        }
        else
        {
            m_Velocity.y = 0;
            //m_Velocity.y *= SLOWDOWN_RATE;
        }*/

       
    //}

    m_Position += m_Velocity * deltaTime;
    m_Sprite.setPosition(m_Position);
    //SetBoundingBox(m_Sprite.getGlobalBounds());
    SetCenter(m_Position.x,m_Position.y);
}
void Player::AdjustPosition(Displayable d)
{
    int index_collision = this->index_collision(d);
    switch (index_collision)
    {
        
    case 0: setGrounded(true); break; // up
    //case 1: setGrounded(false); break; // down
   
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