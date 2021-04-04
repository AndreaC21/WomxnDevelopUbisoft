#pragma once

class BoxCollideable
{

public:
    inline const sf::FloatRect& GetBoundingBox() const { return m_BoundingBox; }

    inline const bool IsColliding(const BoxCollideable& other) const 
    { 
        return m_BoundingBox.intersects(other.GetBoundingBox());
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

    // 0 -> up,1->down,2->left,3->right
    inline const int collisionLeftRight(const BoxCollideable& otherCollider) const
    {
        sf::FloatRect other = otherCollider.GetBoundingBox();
       
        if ((other.top < m_BoundingBox.top && m_BoundingBox.top < (other.top + other.height)))// ||
          //  (other.top < (m_BoundingBox.top + m_BoundingBox.height) && (m_BoundingBox.top + m_BoundingBox.height) < (other.top + other.height)))
        {
            //LEFT
            if (other.left < m_BoundingBox.left && m_BoundingBox.left < (other.left + other.width) )
                return 2;
            //Right
            if (other.left < (m_BoundingBox.left + m_BoundingBox.width)  && (m_BoundingBox.left + m_BoundingBox.width) <(other.left + other.width))
                return 3;
        }

        return -1; 
    }

    inline const int collisionUpDown(const BoxCollideable& otherCollider) const
    {
        sf::FloatRect other = otherCollider.GetBoundingBox();
        if ((other.left < m_BoundingBox.left && m_BoundingBox.left < (other.left + other.width)) ||
            (other.left < (m_BoundingBox.left + m_BoundingBox.width) && (m_BoundingBox.left + m_BoundingBox.width) < (other.left + other.width)))
        {
            // TOP
            if (m_BoundingBox.top <= (other.top + other.height) && m_BoundingBox.top > other.top)
                return 0;
            // DOWN
            if ((m_BoundingBox.top + m_BoundingBox.height) >= other.top && (m_BoundingBox.top + m_BoundingBox.height) <= (other.top + other.height))
                return 1;
        }
        return -1;
    }

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
