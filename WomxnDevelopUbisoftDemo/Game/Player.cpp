#include <stdafx.h>
#include <Game/Player.h>

using namespace sf;

Player::Player() {}

Player::Player(const Player&)
{

}
Player::Player(sf::Vector2f startPosition) : Displayable(startPosition,"Player\\Idle_1.png")
{
    m_Sprite.setScale(0.2f, 0.2f);
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

    /*
    if (m_IsUsingJoystick)
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
    
        if (Keyboard::isKeyPressed(Keyboard::Right))
        {
            m_Velocity.x = fmin(m_Velocity.x + SPEED_INC, SPEED_MAX);
        }
        else if (Keyboard::isKeyPressed(Keyboard::Left))
        {
            m_Velocity.x = fmax(m_Velocity.x - SPEED_INC, -SPEED_MAX);
        }
        else
        {
            m_Velocity.x *= SLOWDOWN_RATE;
        }

       */
    //}

    m_Position += m_Velocity * deltaTime;
    m_Sprite.setPosition(m_Position);
    SetCenter(m_Position);
}

void Player::StartEndGame()
{
	m_IsPlayingEndGame = true;

}