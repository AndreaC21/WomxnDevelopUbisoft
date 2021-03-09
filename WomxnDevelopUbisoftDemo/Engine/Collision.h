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
    inline const int index_collision(const BoxCollideable& other) const
    {
        sf::FloatRect box = other.GetBoundingBox();
        float box_TopRight = box.top + box.width;
        float box_BottomLeft = box.top + box.height;
        float box_BottomRight = box.height + box.width;
        float my_TopRight = m_BoundingBox.top + m_BoundingBox.width;
        float my_BottomRight = m_BoundingBox.height + m_BoundingBox.width;
        float my_BottomLeft = m_BoundingBox.height + m_BoundingBox.top;

        // up
        if (m_BoundingBox.top > box_BottomLeft && m_BoundingBox.top < box_BottomRight &&
            my_TopRight > box_BottomLeft && my_TopRight < box_BottomRight &&
            m_BoundingBox.top <= box_BottomLeft) return 0;
        //down
        if (my_BottomLeft > box.top && my_BottomLeft < box_TopRight &&
            my_BottomRight > box.top && my_BottomRight < box_TopRight &&
            my_BottomLeft >= box.top) return 1;
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
