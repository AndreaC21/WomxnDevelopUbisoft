#include <stdafx.h>
#include <Game/Player.h>

using namespace sf;

#pragma region Player
Player::Player(const Player& p) : Displayable(p)
{
    m_Velocity = p.m_Velocity;
    m_Sprite.setScale(0.3f, 0.3f);
    m_GhostMode = p.m_GhostMode;
    m_BlockDirection = p.m_BlockDirection;
    this->m_CurrentState = p.m_CurrentState;
    m_Position = p.m_Position;
   
    SetBoundingBox(m_Sprite.getGlobalBounds());
}
Player::Player() : Displayable(sf::Vector2f(100,150), "Player\\Idle_1.png")
{
    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));

    m_Sprite_Scale = 0.3f;
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
   // m_Sprite.setOrigin(size *0.5f); // middle
  
    //left,top
    SetBoundingBox(m_Position.x,m_Position.y,size.x*m_Sprite_Scale,size.y*m_Sprite_Scale);

    this->m_CurrentState = new Explorator(m_Position);

  
}
Player::Player(sf::Vector2f p, const char* s) : Displayable(p, s)
{
    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));

    m_Sprite_Scale = 0.3f;
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);

    SetBoundingBox(m_Position.x, m_Position.y, size.x * m_Sprite_Scale, size.y * m_Sprite_Scale);

    m_BlockDirection = new bool[4]{ false,false,false,false};
}
void Player::Update(float deltaTime)
{
    if (m_IsPlayingEndGame)
    {
        return;
    }

    this->m_CurrentState->Update(deltaTime);

    if (Keyboard::isKeyPressed(Keyboard::G))
    {
        Switch();
    }

    m_Position = this->m_CurrentState->getPosition();
  
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
    */
 
}

int Player::AdjustPosition(Displayable* d)
{
   return this->m_CurrentState->AdjustPosition(d);
  
}
void Player::NoCollisionDetected()
{
    this->m_CurrentState->m_BlockDirection[0] = false;
    this->m_CurrentState->m_BlockDirection[1] = false;
    this->m_CurrentState->m_BlockDirection[2] = false;
    this->m_CurrentState->m_BlockDirection[3] = false;
}
void Player::SwitchMode()
{
}
void Player::Switch()
{
    if (typeid(*this->m_CurrentState) == typeid(Explorator))
    {
        //  m_GhostMode = true;
        this->m_CurrentState = new Ghost(m_Position);
    }
    else
    {
        //m_GhostMode = false;
        this->m_CurrentState = new Explorator(m_Position);
    }

    m_Texture = this->m_CurrentState->m_Texture;
    m_Sprite.setTexture(m_Texture);
    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));
    SetBoundingBox(m_Position.x, m_Position.y, size.x * m_Sprite_Scale, size.y * m_Sprite_Scale);
   
}

void Player::StartEndGame()
{
	m_IsPlayingEndGame = true;

}

bool Player::isGhostMode()
{
    return  (typeid(*this->m_CurrentState) == typeid(Ghost));
}
Player* Player::getCurrentState()
{
    return this->m_CurrentState;
}
#pragma endregion

#pragma region Ghost

Ghost::Ghost(sf::Vector2f p) : Player(p, "Ghost\\Idle_1.png")
{
    //m_GhostMode = true;
    m_Duration = 5.0f;
}
Ghost::Ghost(const Ghost& g) : Player(g)
{
    m_Duration = g.m_Duration;
}

void Ghost::Update(float deltaTime)
{
    const float SPEED_MAX = 400.0f;
    const float SPEED_INC = 15.0f;
    const float SLOWDOWN_RATE = 0.9f;

    if (Keyboard::isKeyPressed(Keyboard::Right) && m_BlockDirection[3] == false)
    {
        m_Velocity.x = fmin(m_Velocity.x + SPEED_INC, SPEED_MAX);
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left) && m_BlockDirection[2] == false)
    {
        m_Velocity.x = fmax(m_Velocity.x - SPEED_INC, -SPEED_MAX);
    }
    else
    {
        m_Velocity.x *= SLOWDOWN_RATE;
    }

    if (Keyboard::isKeyPressed(Keyboard::Down) && m_BlockDirection[1]==false)
    {
        m_Velocity.y = fmin(m_Velocity.y + SPEED_INC, SPEED_MAX);

    }
    else if (Keyboard::isKeyPressed(Keyboard::Up) && m_BlockDirection[0] == false)
    {
        m_Velocity.y = fmax(m_Velocity.y - SPEED_INC, -SPEED_MAX);

    }
    else
    {
        m_Velocity.y *= SLOWDOWN_RATE;
    }

    m_Position += m_Velocity * deltaTime;
    m_Sprite.setPosition(m_Position);
    SetCenter(m_Position.x, m_Position.y);
}

int Ghost::AdjustPosition(Displayable* d)
{
    if (typeid(*d) == typeid(Obstacle))
    {
        Obstacle* o = static_cast <Obstacle*>(d);
        if (o->canGhostTraverse()) return -1;

    }

    int index_collision = this->index_collision(*d);

    switch (index_collision)
    {
        case 0: m_Velocity.y = 0; m_BlockDirection[0] = true; break; // up
        case 1: m_Velocity.y = 0; m_BlockDirection[1] = true; break; // down
        case 2: m_Velocity.x = 0; m_BlockDirection[2] = true; break; // left
        case 3: m_Velocity.x = 0; m_BlockDirection[3] = true; break; //right
        default:
            m_BlockDirection[0] = false;
            m_BlockDirection[1] = false;
            m_BlockDirection[2] = false;
            m_BlockDirection[3] = false;
            break;

    }
    return index_collision;
}

#pragma endregion

#pragma region Explorator
Explorator::Explorator(sf::Vector2f p) : Player(p, "Player\\Idle_1.png")
{
    m_onGround = false;
    
    m_lifePoint_max = 100.0f;
    m_lifePoint = m_lifePoint_max;
    m_attack = 25.0f;
    m_maxThrowableWeapon = 5;
    m_currentThrowableWeapon = 0;
    m_DurationShoot = 0.1f;
    m_TimePreviousShoot = 0.0f;
    m_CanShoot = true;

}
Explorator::Explorator(const Explorator& p) : Player(p)
{
    m_onGround = p.m_onGround;
   
    m_lifePoint = p.m_lifePoint;
    m_lifePoint_max = p.m_lifePoint_max;
    m_attack = p.m_attack;
}
void Explorator::Update(float deltaTime)
{
    const float SPEED_MAX = 300.0f;
    const float SPEED_INC = 10.0f;
    const float SLOWDOWN_RATE = 0.9f;
    const float JUMP_MAX = 600.0f;

    if (Keyboard::isKeyPressed(Keyboard::Right)&& m_BlockDirection[3]==false)
    {
        m_Velocity.x = fmin(m_Velocity.x + SPEED_INC, SPEED_MAX);
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left) &&m_BlockDirection[2]==false)
    {
        m_Velocity.x = fmax(m_Velocity.x - SPEED_INC, -SPEED_MAX);
    }
    else
    {
        m_Velocity.x *= SLOWDOWN_RATE;
    }
    if (Keyboard::isKeyPressed(Keyboard::Space) && m_onGround)
    {
        m_Velocity.y = -JUMP_MAX;
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

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && m_CanShoot )
    {
        if (m_currentThrowableWeapon < m_maxThrowableWeapon)
        {
            m_TimePreviousShoot = clock.asSeconds();
            m_currentThrowableWeapon++;
            m_CanShoot = false;
            m_listWeapon.push_back(Weapon(GetCenter(), true, m_TimePreviousShoot));
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
  
    m_Position += m_Velocity * deltaTime;
    m_Sprite.setPosition(m_Position);
  
    SetCenter(m_Position.x, m_Position.y);
}
void Explorator::UpdateWeapon(Displayable* d)
{
    for (int j = 0; j < m_listWeapon.size(); ++j)
    {
        if (m_listWeapon[j].IsColliding(*d))
        {
            WeaponTouch(j, d);
        }
    }
}
void Explorator::UpdateWeapon(Ennemy& e)
{
    for (int j = 0; j < m_listWeapon.size(); ++j)
    {
        if (m_listWeapon[j].IsColliding(e))
        {
            WeaponTouch(j, e);
        }
    }
}
int Explorator::AdjustPosition(Displayable* d)
{
    int index_collision = this->index_collision(*d);

    switch (index_collision)
    {

    case 0: setGrounded(false); m_Velocity.y = 0; m_BlockDirection[0] = true; break; // up
    case 1: setGrounded(true); m_Velocity.y = 0; m_BlockDirection[1] = true; break; // down
    case 2: m_Velocity.x = 0;  m_BlockDirection[2] = true; break;
    case 3: m_Velocity.x = 0;  m_BlockDirection[3] = true; break;
  
    default:
        m_BlockDirection[0] = false;
        m_BlockDirection[1] = false;
        m_BlockDirection[2] = false;
        m_BlockDirection[3] = false;
        break;
    }

    return index_collision;
}

std::string Explorator::getLifePoint() const
{
    return std::to_string(m_lifePoint) + "/" + std::to_string(m_lifePoint_max);
}
float Explorator::getCurrentLifePoint() const
{
    return this->m_lifePoint;
}
void Explorator::loseLifePoint(float amount)
{
    this->m_lifePoint -= amount;
}
std::vector<Weapon> Explorator::getWeapon() const
{
    return this->m_listWeapon;
}
void Explorator::UpdateShoot(sf::Time currentTime)
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

bool Explorator::WeaponTouch(int index_weapon, Displayable* d)
{
    return m_listWeapon[index_weapon].TouchDisplayable(d);
}

bool Explorator::WeaponTouch(int index_weapon, Ennemy& e)
{
    return m_listWeapon[index_weapon].TouchEnnemy(e);
}

bool Explorator::isGrounded() const
{
    return m_onGround;
}
void Explorator::setGrounded(bool b)
{
    m_onGround = b;
}






#pragma endregion
