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
    this->m_CurrentState = p.m_CurrentState;
    m_Position = p.m_Position;
   
    SetBoundingBox(m_Sprite.getGlobalBounds());
}
Player::Player()
{
    this->m_CurrentState = new Explorator(sf::Vector2f(100,100));

}
Player::Player(sf::Vector2f p, const char* s) : Displayable(p, s)
{
    const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));

    m_Sprite_Scale = 0.3f;
    m_Sprite.setScale(m_Sprite_Scale, m_Sprite_Scale);
    m_Sprite.setOrigin(size.x * 0.5f, 0);
   // SetBoundingBox(m_Position.x - size.x, m_Position.y, size.x * m_Sprite_Scale, size.y * m_Sprite_Scale);
   
    m_BlockDirection = new bool[4]{ false,false,false,false};
   
    SetBoundingBox(m_Sprite.getGlobalBounds());

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
  
}
void Player::UpdateCollisionWithDisplayable(std::vector<Displayable*> listDisplayable)
{
    //ResetCollision
    this->m_CurrentState->m_BlockDirection = new bool[4]{ false,false,false,false };

    for (int i = 0; i < listDisplayable.size(); ++i)
    {
        if (this->m_CurrentState->IsColliding(*listDisplayable[i]))
        {
            this->m_CurrentState->AdjustPosition(listDisplayable[i]);
        }

        if (isGhostMode() == false)
        {
            Explorator* e = dynamic_cast <Explorator*>(m_CurrentState);
            e->CheckWeaponCollisionWithDisplayable(listDisplayable[i]);
        }
    }
}
void Player::UpdateWeaponCollisionWith(std::vector<Ennemy>& listEnnemy)
{

    if (isGhostMode() == true)
    {
        return;
    }
    Explorator* e = dynamic_cast <Explorator*>(m_CurrentState);
    for (int i = 0; i < listEnnemy.size(); ++i)
    {
        e->CheckWeaponCollisionWithEnnemy(listEnnemy[i]);
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
    if (typeid(*this->m_CurrentState) == typeid(Explorator))
    {
        this->m_CurrentState = new Ghost(m_Position);
    }
    else
    {
        this->m_CurrentState = new Explorator(m_Position);
    }

    m_Texture = this->m_CurrentState->m_Texture;
    m_Sprite.setTexture(m_Texture);
    SetBoundingBox(m_Sprite.getGlobalBounds());
    //const sf::Vector2f size(static_cast<float>(m_Texture.getSize().x), static_cast<float>(m_Texture.getSize().y));
   // SetBoundingBox(m_Position.x, m_Position.y, size.x * m_Sprite_Scale, size.y * m_Sprite_Scale);
   
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
#pragma endregion

#pragma region Ghost

Ghost::Ghost(sf::Vector2f p) : Player(p, "Ghost\\Idle_1.png")
{
    SPEED_MAX = 400.0f;
    m_Duration = 5.0f;
}
Ghost::Ghost(const Ghost& g) : Player(g)
{
    m_Duration = g.m_Duration;
}

void Ghost::Update(float deltaTime)
{
    if (Keyboard::isKeyPressed(Keyboard::Right) && m_BlockDirection[eDirection::Right] == false)
    {
        m_Velocity.x = SPEED_MAX;
        FlipSprite(true);
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left) && m_BlockDirection[eDirection::Left] == false)
    {
        m_Velocity.x = -SPEED_MAX;
        FlipSprite(false);
    }
    else
    {
        m_Velocity.x *= 0;
    }

    if (Keyboard::isKeyPressed(Keyboard::Down) && m_BlockDirection[eDirection::Bottom]==false)
    {
        m_Velocity.y = SPEED_MAX;

    }
    else if (Keyboard::isKeyPressed(Keyboard::Up) && m_BlockDirection[eDirection::Top] == false)
    {
        m_Velocity.y = -SPEED_MAX;

    }
    else
    {
        m_Velocity.y *= 0;
    }

    m_Position += m_Velocity * deltaTime;
    m_Sprite.setPosition(m_Position);
    SetBoundingBox(m_Sprite.getGlobalBounds());
}

int Ghost::AdjustPosition(Displayable* d)
{
    if (typeid(*d) == typeid(Obstacle))
    {
        Obstacle* o = dynamic_cast<Obstacle*>(d);
        if (o != nullptr && o->canGhostTraverse()) return -1;
    }

    int index_collision = this->CollisionDirection(*d);
    if (index_collision != -1)
    {
        setCollision(index_collision);
    }
    return index_collision;
}

#pragma endregion

#pragma region Explorator
Explorator::Explorator(sf::Vector2f p) : Player(p, "Player\\Idle_1.png")
{
    m_lifePoint_max = 100.0f;
    m_lifePoint = m_lifePoint_max;
    m_attack = 25.0f;
    m_maxThrowableWeapon = 5;
    m_currentThrowableWeapon = 0;
    m_DurationShoot = 0.1f;
    m_TimePreviousShoot = 0.0f;
    SPEED_MAX = 300;
    m_IsDead = false;

}
Explorator::Explorator(const Explorator& p) : Player(p)
{
    m_lifePoint = p.m_lifePoint;
    m_lifePoint_max = p.m_lifePoint_max;
    m_attack = p.m_attack;
    m_IsDead = p.m_IsDead;
}
void Explorator::Update(float deltaTime)
{
    
    const float SPEED_INC = 10.0f;
   
    if (isGrounded()==false || m_BlockDirection[eDirection::Top]==true)
    {
        //Fall
        m_Velocity.y = fmin(m_Velocity.y + SPEED_INC, JUMP_MAX);
    }

    if (Keyboard::isKeyPressed(Keyboard::Right)&& m_BlockDirection[eDirection::Right]==false)
    {
        m_Velocity.x = SPEED_MAX;      
        FlipSprite(true);
    }
    else if (Keyboard::isKeyPressed(Keyboard::Left) && m_BlockDirection[eDirection::Left]==false)
    {
        m_Velocity.x = -SPEED_MAX;
        FlipSprite(false);
    }
    else
    {
        m_Velocity.x *= 0;
    }
    if (Keyboard::isKeyPressed(Keyboard::Space) && isGrounded())
    {
        m_Velocity.y = -JUMP_MAX;
    }

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && isShootAvailable() )
    {
        if (m_currentThrowableWeapon < m_maxThrowableWeapon)
        {
            m_TimePreviousShoot = m_clock.getElapsedTime().asSeconds();
            m_currentThrowableWeapon++;
           
            bool direction = m_Sprite.getScale().x >0; 
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
  
    m_Position += m_Velocity * deltaTime;
    m_Sprite.setPosition(m_Position);
    SetBoundingBox(m_Sprite.getGlobalBounds());
}

bool Explorator::isShootAvailable() const
{
    if (m_clock.getElapsedTime().asSeconds() > this->m_TimePreviousShoot + this->m_DurationShoot)
    {
        return true;
    }
    return false;
}
void Explorator::CheckWeaponCollisionWithDisplayable(Displayable* d)
{
    for (int j = 0; j < m_listWeapon.size(); ++j)
    {
        if (m_listWeapon[j].IsColliding(*d))
        {
            m_listWeapon[j].TouchDisplayable(d);
        }
    }
}
void Explorator::CheckWeaponCollisionWithEnnemy(Ennemy& e)
{
    for (int j = 0; j < m_listWeapon.size(); ++j)
    {
        if (m_listWeapon[j].IsColliding(e))
        {
            m_listWeapon[j].TouchEnnemy(e);
        }
    }
}
int Explorator::AdjustPosition(Displayable* d)
{
    if (typeid(*d) == typeid(Portal))
    {
        return -1;
    }

    int index_collision = this->CollisionDirection(*d);
    if (index_collision != -1)
    {
        setCollision(index_collision);
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

std::vector<Weapon> Explorator::getWeapons()
{
    return m_listWeapon;
}
bool Explorator::isGrounded() const
{
    return this->m_BlockDirection[eDirection::Bottom] == true;
}
bool Explorator::isDead() const
{
    return this->m_lifePoint <= 0.0f;
}


#pragma endregion
