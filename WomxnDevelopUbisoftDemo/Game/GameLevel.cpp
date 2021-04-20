#include <stdafx.h>
#include "GameLevel.h"


#pragma region GameLevel

int GameLevel::max_size_y = Level::grid_size * Case::size_pixel_y;
int GameLevel::max_size_x = Level::grid_size * Case::size_pixel_x;

GameLevel::GameLevel() : Game{ "Level 1" },
m_Background({ 0,0 }, "Background.png",7.0f),
m_Player(),
m_level()
{
    generateLevel();
    generateEnnemy();
    m_IsFinished = false;
    m_ExecuteEndGame = false;
    m_clock.restart();
}

void GameLevel::Update(float deltaTime)
{
    if (isGameFinish()==false)
    { 
        m_Player.UpdateCollisionWithDisplayable(m_level.GetAllDisplayable());
        m_Player.UpdateWeaponCollisionWith(m_listEnnemy);
        m_Player.Update(deltaTime);
        UpdateEnnemy(deltaTime);
  
        m_View.setCenter(m_Player.getPosition());
        m_Window.setView(m_View);

    }
    // Game finish player find exit
    else
    {
        if (m_ExecuteEndGame == false)
        {
            m_ExecuteEndGame = true;
            if (m_PlayerSucceed)
            {
                StartEndGameSuccess();
            }
            else
            {
                StartEndGameFail();
            }
            m_Player.StartEndGame();
        }
    }
}

void GameLevel::UpdateEnnemy(float deltaTime)
{
    for (int i = 0; i < m_listEnnemy.size(); ++i)
    {
        m_listEnnemy[i].Update(deltaTime);

        if (m_listEnnemy[i].isGrounded() == false)
        {
            m_listEnnemy[i].Fall();

            for (auto& p : this->m_level.getColumnsPlatform(m_listEnnemy[i].getSpawnedColumns(), m_listEnnemy[i].getSpawnedRow()))
            {
                if (m_listEnnemy[i].IsColliding(*p))
                {
                    m_listEnnemy[i].StopFall();
                }
            }
        }
        if (m_listEnnemy[i].Dead())
        {
            m_listEnnemy.erase(std::find(m_listEnnemy.begin(), m_listEnnemy.end(), m_listEnnemy[i]));
        }
    }
}
void GameLevel::Render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));
    target.draw(m_Background);

    target.draw(m_Player);
    
    for (int i = 0; i < m_level.GetAllDisplayable().size(); ++i)
    {
        target.draw(*m_level.GetAllDisplayable()[i]);
    }
    if (m_Player.isGhostMode() == false)
    {
        for (int i = 0; i < m_Player.getWeapons().size(); ++i)
        {
            target.draw(m_Player.getWeapons()[i]);
        }
    }
    
    for (int i = 0; i < m_listEnnemy.size(); ++i)
    {
        target.draw(m_listEnnemy[i]);
    }
    if (m_IsFinished)
    {
        target.draw(m_EndGameSprite);
        target.draw(m_EndGameTextTime);
    }
}

void GameLevel::RenderDebugMenu(sf::RenderTarget& target)
{
    ImGui::Begin("Debug Menu");
    ImGui::Text("Press F1 to close this debug menu");
    ImGui::NewLine();

    if (ImGui::CollapsingHeader("Collision"))
    {
        ImGui::Text("top: %d"+m_Player.getBlockDirection());
    }
    if (ImGui::CollapsingHeader("Weapon"))
    {
        ImGui::Text("%d ", (int)m_Player.getWeapons().size()); 
    }
    if (ImGui::CollapsingHeader("Player"))
    {
       ImGui::Text("%s", m_Player.getLifePoint().c_str());
    }
    ImGui::End();
}

void GameLevel::generateLevel()
{
    m_View.reset(sf::FloatRect(0, 0, 1024, 768));
    m_Window.setView(m_View);
}
void GameLevel::generateEnnemy()
{
    m_listEnnemy.push_back(Ennemy(0,0, Case::size_pixel_x, Case::size_pixel_y,&m_Player));
  // m_listEnnemy.push_back(Ennemy(1,2, Case::size_pixel_x, Case::size_pixel_y, &m_Player));
  //  m_listEnnemy.push_back(Ennemy(3,3, Case::size_pixel_x, Case::size_pixel_y, &m_Player));
   // m_listEnnemy.push_back(Ennemy(5, 1, Case::size_pixel_x, Case::size_pixel_y, &m_Player));

}

bool GameLevel::isGameFinish()
{
    if (m_Player.isGhostMode() == false)
    {
        if (m_Player.IsColliding(*m_level.getPortal()))
        {
            m_PlayerSucceed = true;
            m_IsFinished = true;

            return true;
        }
        else if (m_Player.isDead())
        {
            m_PlayerSucceed = false;
            m_IsFinished = true;

            return true;
        }
    }
    return false;
}
void GameLevel::StartEndGameSuccess()
{
    m_TextFont.loadFromFile(".\\Assets\\Font\\arial.ttf");
    // Preparation for endgame
    m_EndGameTextTime.setFont(m_TextFont);
    m_EndGameTextTime.setCharacterSize(24);
    m_EndGameTextTime.setFillColor(sf::Color::Red);
    m_EndGameTextTime.setStyle(sf::Text::Bold);
    m_EndGameTextTime.setPosition(m_Player.getPosition().x, m_Player.getPosition().y -250);

    m_EngameTexture.loadFromFile(".\\Assets\\EndGame\\Success.png");

    m_EndGameTime = m_clock.getElapsedTime().asSeconds();
    m_EndGameTextTime.setString("Time to complete level: " + std::to_string(m_EndGameTime));
    m_EndGameSprite.setTexture(m_EngameTexture);
    m_EndGameSprite.setPosition(m_Player.getPosition().x, m_Player.getPosition().y - 100);
}
void GameLevel::StartEndGameFail()
{
    m_EngameTexture.loadFromFile(".\\Assets\\EndGame\\Fail.png");
    m_EndGameSprite.setTexture(m_EngameTexture);
    m_EndGameSprite.setPosition(m_Player.getPosition().x, m_Player.getPosition().y -100);
}

#pragma endregion

#pragma region Case

int Case::size_pixel_x = 600;
int Case::size_pixel_y = 300;

Case::Case()
{
    m_empty = true;
    x = 0;
    y = 0;
}
Case::Case(int x, int y)
{
    this->x = x;
    this->y = y;
    m_empty = true;
}
Case::Case(const Case& c)
{
    m_empty = c.m_empty;
    this->x = c.x;
    this->y = c.y;
    if (m_empty == false)
    {
         for (int i = 0; i < c.m_listObstacle.size(); ++i)
        {
            m_listObstacle.push_back(c.m_listObstacle[i]);
        }
        for (int i = 0; i < c.m_listPlatform.size(); ++i)
        {
            m_listPlatform.push_back(c.m_listPlatform[i]);
        }
        this->portal = c.portal;
    }
   
}
void Case::set(int x, int y)
{
    this->x = x;
    this->y= y;
}

int Case::getX() const {return this->x;}
int Case::getY() const {return this->y;}

Portal& Case::getPortal() { return this->portal; }

std::vector<Platform>& Case::getAllPlatform() 
{ 
    return this->m_listPlatform; 
}
std::vector<Obstacle>& Case::getAllObstacle() 
{
    return this->m_listObstacle;
}

bool Case::hasObstacle(Platform::Position direction)
{
    if (this->m_empty == false)
    {
        for (int i = 0; i < this->getAllPlatform().size(); ++i)
        {
            if (this->getAllPlatform()[i].getPos() == direction)
            {
                return true;
            } 
        }
    }
    return false;
}
void Case::addPlatform(Platform p)
{
    this->m_empty = false;
    this->m_listPlatform.push_back(p);
}

void Case::addObstacle(Obstacle o)
{
    this->m_empty = false;
    this->m_listObstacle.push_back(o); 
}
void Case::addPortal(Portal p)
{
    this->m_empty = false;
    this->portal = p;
}

bool Case::isEmpty() const
{
    return this->m_empty;
}
#pragma endregion

#pragma region Level

int Level::grid_size = 8;

Level::Level()
{
    this->n = grid_size;
    this->m = grid_size;

    /* Allocation dynamique */
    grid = new Case * [n];
    for (int i = 0; i < n; ++i)
    {
        grid[i] = new Case[m];
    }
    for (int i = 0; i < n; ++i)
    {
        for (int j = 0; j < m; ++j)
        {
            grid[i][j].set(i, j);
        }  
    }
    genereLevel();
}
Level::Level(const Level&l)
{
    this->n = l.getSize_n();
    this->m = l.getSize_m();
    grid = new Case * [this->n];
    for (int i = 0; i < n; ++i)
    {
        grid[i] = new Case[this->m];
        for (int j = 0; j < m; ++j)
        {
            this->grid[i][j] = l.at(i, j);
        }
    }
   
}

Case Level::at(int i, int j) const
{
    return grid[i][j];
}

Level::~Level()
{
    for (int i = 0; i < n; ++i)
    {
        delete [] grid[i];
    }
    delete [] grid;
}
std::vector<Displayable*> Level::GetAllDisplayable() const
{
    return m_listDisplayable;
}
void Level::SetPlatform(int i, int j, float rotation) const
{
    grid[i][j].addPlatform( Platform(i, j, Case::size_pixel_x, Case::size_pixel_y,rotation));
}
void Level::SetObstacle(int i, int j,bool traversable) const
{
    grid[i][j].addObstacle(Obstacle(i, j, Case::size_pixel_x, Case::size_pixel_y,traversable));
}
void Level::SetPortal(int i, int j)
{
    grid[i][j].addPortal(Portal(i, j, Case::size_pixel_x, Case::size_pixel_y));
    m_Portal = &(grid[i][j].getPortal());
}
int Level::getSize_n() const
{
    return this->n;
}
int Level::getSize_m() const
{
    return this->m;
}
std::vector<Platform*> Level::getColumnsPlatform(int columToCheck,int rowToBegin )
{
    std::vector<Platform*> listDisplayable;
    for (int j = rowToBegin+1; j < getSize_m(); ++j)
    {
        if (this->grid[columToCheck][j].isEmpty() == false)
        { 
            for (Platform& p : this->grid[columToCheck][j].getAllPlatform())
            {
                if ( p.getPos() == Platform::Position::TOP)
                {
                    listDisplayable.push_back(&p);
                }
            }
        }
    }
    return listDisplayable;
}

Portal* Level::getPortal() const
{
    return this->m_Portal;
}
void Level::genereLevel()
{
    if (n <= 0 || m <= 0) return;

    
    for (int i = 0; i < Level::grid_size; ++i)
    {
        this->SetPlatform(i, 0); // TOP BORDER
        this->SetPlatform(i, Level::grid_size-1);  //BOTTOM BORDER
        this->SetPlatform(0, i, 90.0f); // LEFT BORDER
        this->SetPlatform(Level::grid_size-1, i, -90.0f); // RIGHT BORDER
    }
    
    this->SetObstacle(1, 0, true);

    this->SetPlatform(0, 1);

    this->SetPlatform(0, 1); this->SetPlatform(0, 3); this->SetPlatform(0, 4); this->SetPlatform(0, 6);
    this->SetPlatform(1, 2); this->SetPlatform(1, 3); this->SetPlatform(1, 5);
    this->SetPlatform(2, 1); this->SetPlatform(2, 3); this->SetPlatform(2, 4); this->SetPlatform(2, 6);
    this->SetPlatform(3, 2); this->SetPlatform(3, 4); this->SetPlatform(3, 5);
    this->SetPlatform(4, 1); this->SetPlatform(4, 2); this->SetPlatform(4, 3); this->SetPlatform(4, 4); this->SetPlatform(4, 6);
    this->SetPlatform(5, 1); this->SetPlatform(5, 6);
    this->SetPlatform(6, 1); this->SetPlatform(6, 2); this->SetPlatform(6, 3); this->SetPlatform(6, 4); this->SetPlatform(6, 6);
    this->SetPlatform(7, 4); this->SetPlatform(7, 6);
    
    this->SetPortal(5, 5);

    BuildListDisplayable();
   
}

void Level::BuildListDisplayable()
{
    for (int i = 0; i < this->n; ++i)
    {
        for (int j = 0; j < this->m; ++j)
        {
            if (this->at(i, j).isEmpty() == false)
            {
                for (int h = 0; h < this->at(i, j).getAllPlatform().size(); ++h)
                {
                    m_listDisplayable.push_back(&(grid[i][j].getAllPlatform()[h]));
                }
                for (int h = 0; h < this->at(i, j).getAllObstacle().size(); ++h)
                {
                    m_listDisplayable.push_back(&(grid[i][j].getAllObstacle()[h]));
                }
            }
        }
    }
}
bool Contains(std::vector<std::pair<Case, Level::Direction>> list, std::pair<Case, Level::Direction> element)
{
    for (int i = 0; i < list.size(); ++i)
    {
        if (list.at(i).first == element.first && list.at(i).second == element.second)
        {
            return true;
        }
    }
    return false;
}
Case Level::searchHorizontale(Case pos, int horizontaleDirection, float distance,Case destination)
{
    int x = pos.getX();
    int y = pos.getY();

    int x1;

    bool findDirection = false;

    while (findDirection == false)
    {

        x1 = x + horizontaleDirection;
        Case newCase(x1, y);
        if (Contains(open, std::pair<Case, Direction>(newCase, Direction(1, 0))) == false)
        {
            return Case();
        }

        if (newCase.hasObstacle(Platform::Position::RIGHT))
        {
            return Case();
        }

        if (newCase == destination)
        {
            open.push_back(std::pair<Case, Direction>(newCase, Direction(1, 0)));
        }

        distance += 1;
        int x2 = x1 + horizontaleDirection;

        return newCase;

    }
    return Case();
     
}


#pragma endregion


