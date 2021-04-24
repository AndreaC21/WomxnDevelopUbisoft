#pragma once

enum eDirection
{
    Top = 0,
    Bottom = 1,
    Left = 2,
    Right = 3,
    Null = -1

};

class Player;
class Platform;
class Ennemy;
class Obstacle;
class Displayable;

class BoxCollideable
{

public:
    inline const sf::FloatRect& GetBoundingBox() const { return m_BoundingBox; }

    inline const bool IsColliding(const BoxCollideable& other) const 
    { 
        return m_BoundingBox.intersects(other.GetBoundingBox());
    }
    inline const bool IsColliding(const std::vector<BoxCollideable> others) const
    {
        for (int i = 0; i < (int)others.capacity(); ++i)
        {
            if (IsColliding(others[i]))
                return true;
        }
        return false;
    }
    inline const bool Contains(float x, float y) const { return m_BoundingBox.contains(x, y); }
    inline const bool Contains(const sf::Vector2f& pos) const { return m_BoundingBox.contains(pos); }
    inline const bool Contains(const BoxCollideable& other) const
    {
        auto bbox = other.GetBoundingBox();
        return m_BoundingBox.contains(bbox.left, bbox.top) && m_BoundingBox.contains(bbox.left + bbox.width, bbox.top + bbox.height);
    }

    inline const sf::Vector2f GetCenter() 
    {
        return sf::Vector2f(m_BoundingBox.left + (m_BoundingBox.width / 2.0f), m_BoundingBox.top + (m_BoundingBox.height / 2.0f));
    }

    inline const eDirection CollisionDirection(const BoxCollideable& otherCollider)
    {
        sf::FloatRect other = otherCollider.GetBoundingBox();

        if ((other.top +(other.height*0.5f))<= m_BoundingBox.top && m_BoundingBox.top <= (other.top + other.height))
            return eDirection::Top;
        if (other.top <= (m_BoundingBox.top + m_BoundingBox.height) && (m_BoundingBox.top + m_BoundingBox.height) <= (other.top + other.height * 0.5f))
            return eDirection::Bottom;
        if ((other.left + other.width * 0.5f) < m_BoundingBox.left && m_BoundingBox.left <= (other.left + other.width))
            return eDirection::Left;
        if (other.left <= (m_BoundingBox.left + m_BoundingBox.width) && (m_BoundingBox.left + m_BoundingBox.width) <= (other.left + other.width * 0.5f))
            return eDirection::Right;

        return eDirection::Null;
    }

   /* virtual void OnCollide(Obstacle&) {};
    virtual void OnCollide(Player&) {};
    virtual void OnCollide(Ennemy&) {};
    virtual void OnCollide(Platform&) {};
    virtual void OnCollide(Displayable*&) {};*/
   // virtual void OnCollide(BoxCollideable&) {};

protected:
    inline void SetBoundingBox(float left, float top, float width, float height)
    {
        m_BoundingBox.left = left;
        m_BoundingBox.top = top;
        m_BoundingBox.width = width;
        m_BoundingBox.height = height;
    }

    inline void SetBoundingBox(const sf::Vector2f& center, const sf::Vector2f& size)
    {
        m_BoundingBox.left = center.x - (size.x / 2.0f);
        m_BoundingBox.top = center.y - (size.y / 2.0f);
        m_BoundingBox.width = size.x;
        m_BoundingBox.height = size.y;
    }

    inline void SetBoundingBox(const sf::FloatRect r)
    {
        m_BoundingBox.left = r.left;
        m_BoundingBox.top = r.top;
        m_BoundingBox.width = r.width;
        m_BoundingBox.height = r.height;
    }

    inline void SetCenter(const sf::Vector2f& center)
    {
        m_BoundingBox.left = center.x - (m_BoundingBox.width / 2.0f);
        m_BoundingBox.top = center.y - (m_BoundingBox.height / 2.0f);
    }
    inline void SetCenter(const float& x, const float& y)
    {
        m_BoundingBox.left = x;
        m_BoundingBox.top = y;
    }

    sf::FloatRect m_BoundingBox;
};


class CollisionInfo
{
public:

    enum class eEntityType
    {
        Player = 0,
        Ennemy = 1,
        Obstacle = 2,
        Platform = 3,
        Portal = 4,
        Weapon = 5,
        Undefined = -1
    };

    CollisionInfo(BoxCollideable collided, eEntityType collidedType)
    {
        m_Collided = collided;
        m_CollidedType = collidedType;
    }
    // GET
    eEntityType GetCollidedType()
    {
        return m_CollidedType;
    }

    BoxCollideable GetCollided()
    {
        return m_Collided;
    }


    //SET
    void SetCollidedType(eEntityType e)
    {
        m_CollidedType = e;
    }

    void GetCollided(BoxCollideable c)
    {
        m_Collided = c;
    }

private:
    eEntityType m_CollidedType;
    BoxCollideable m_Collided;
};
class ICollideable
{
    virtual void OnCollide() = 0;
};
