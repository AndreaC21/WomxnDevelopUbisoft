#include <stdafx.h>
#include <Game/Platform.h>

using namespace sf;

Platform::Platform() : Displayable(sf::Vector2f(0, 500), "Platform\\2.png")
{
    m_Sprite.setScale(0.3f, 0.3f);
}
void Platform::Update(float deltaTime)
{

    //}

    //m_Position += m_Velocity * deltaTime;
    //m_Sprite.setPosition(m_Position);
    //SetCenter(m_Position);
}