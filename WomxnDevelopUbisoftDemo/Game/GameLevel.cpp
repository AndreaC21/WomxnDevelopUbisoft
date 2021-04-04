#include <stdafx.h>
#include "GameLevel.h"

bool b = false;
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

    buildListPlatform();
    m_IsFinished = false;
    m_ExecuteEndGame = false;
    m_clock.restart();
}

void GameLevel::Update(float deltaTime)
{
    if (isGameFinish()==false)
    {
        int counter = 0;
        
        for (int i = 0; i < list_displayable.size(); ++i)
        {
            if (m_Player.getCurrentState()->IsColliding(*list_displayable[i]))
            {
                m_Player.AdjustPosition(list_displayable[i]);
            }
            else
            {
                counter++;
            }
        
            if (m_Player.isGhostMode()==false)
            {
                Explorator* o = static_cast <Explorator*>(m_Player.getCurrentState());
                o->UpdateWeapon(list_displayable[i]);
            }
       
        }
        if (m_Player.isGhostMode() == false)
        {
            Explorator* o = static_cast <Explorator*>(m_Player.getCurrentState());
            o->UpdateShoot(m_clock.getElapsedTime());
        }
    
        m_Player.Update(deltaTime); 
    
        for (int i =0; i < m_listEnnemy.size() ;++i)
        {
            m_listEnnemy[i].Update(deltaTime);

            if (m_Player.isGhostMode() == false)
            {
                Explorator* o = static_cast <Explorator*>(m_Player.getCurrentState());
                o->UpdateWeapon(m_listEnnemy[i]);
            }
         
            if (m_listEnnemy[i].isGrounded() == false)
            {
                m_listEnnemy[i].Fall();
                
                for (auto& p : this->m_level.getColumnsPlatform(m_listEnnemy[i].getSpawnedColumns(),m_listEnnemy[i].getSpawnedRow()))
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
        if (counter == list_displayable.size())
        {
            m_Player.getCurrentState()->NoCollisionDetected(0);
            m_Player.getCurrentState()->NoCollisionDetected(2);
            //m_player.setGrounded(false);
        }

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

void GameLevel::Render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));
    target.draw(m_Background);

   // target.draw(m_player);
    target.draw(*m_Player.getCurrentState());
    
    for (int i = 0; i < list_displayable.size(); ++i)
    {
        target.draw(*list_displayable[i]);
    }
    if (m_Player.isGhostMode() == false)
    {
        Explorator* e = static_cast <Explorator*>(m_Player.getCurrentState());

        for (int i = 0; i < e->getWeapon().size(); ++i)
        {
            target.draw(e->getWeapon()[i]);
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

    if (ImGui::CollapsingHeader("Player Statut"))
    {
        ImGui::Text("X: %f Y: %f", m_Player.getPosition().x, m_Player.getPosition().y);
        if (m_Player.isGhostMode())
        {
            ImGui::TextColored(ImVec4(0, 255.f, 0.f, 1.f), "Ghost");
        }
        else
        {
            ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "Explorator");
        }
        ImGui::Text("type: %s", typeid(*m_Player.getCurrentState()).name());
        if (typeid(*m_Player.getCurrentState()) == typeid(Explorator))
        {
            Explorator* e = static_cast<Explorator*>(m_Player.getCurrentState());
            //e->loseLifePoint(1);
            ImGui::Text("LF: %s", e->getLifePoint().c_str());
        }

        ImGui::Text("Block: up: %d - down: %d - left: %d - right: %d", m_Player.getCurrentState()->m_BlockDirection[0] , m_Player.getCurrentState()->m_BlockDirection[1], m_Player.getCurrentState()->m_BlockDirection[2], m_Player.getCurrentState()->m_BlockDirection[3]);
        ImGui::Text("Grounded?:%d", m_Player.getCurrentState()->m_BlockDirection[1]);
    }

 
    if (ImGui::CollapsingHeader("Game status"))
    {
        if (m_IsFinished)
        {
            ImGui::TextColored(ImVec4(0, 255.f, 0.f, 1.f), "Game complete");
        }
        else
        {
            ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "Game not complete");
        }
        
        //ImGui::Text("Portal position: %f,%f", m_level.getPortal()->getPosition().x, m_level.getPortal()->getPosition().y);

    }
    if (ImGui::CollapsingHeader("Debug"))
    {
        //ImGui::Text("%d: ",(int) m_level.getContainsCaseAt(0, 1).size());

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
void GameLevel::buildListPlatform()
{
    // GET ALL PLATFORM AND OBSTACLE FROM THE GRID 
    for (int i = 0; i < m_level.getSize_n() ;++i)
    {
        for (int j = 0; j < m_level.getSize_n(); ++j)
        {
            if (m_level.at(i, j).isEmpty() == false)
            {
                for (int h = 0; h < m_level.getContainsCaseAt(i,j).size(); ++h)
                {
                    list_displayable.push_back(m_level.getContainsCaseAt(i,j)[h]);
                   
                }
            }
        }
    }
}
bool GameLevel::isGameFinish()
{
    if (m_Player.isGhostMode() == false)
    {
        if (m_Player.getCurrentState()->IsColliding(*m_level.getPortal()))
        {
            m_PlayerSucceed = true;
            m_IsFinished = true;

            return true;
        }
        Explorator* e = static_cast<Explorator*>(m_Player.getCurrentState());
        if (e->isDead())
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
        for (int i = 0; i < c.listDisplayable.size(); ++i)
        {
            listDisplayable.push_back(c.listDisplayable[i]);
        }
        for (int i = 0; i < c.listObstacle.size(); ++i)
        {
            listObstacle.push_back(c.listObstacle[i]);
        }
        for (int i = 0; i < c.listPlatform.size(); ++i)
        {
            listPlatform.push_back(c.listPlatform[i]);
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

bool Case::hasObstacle(Platform::Position direction)
{
    if (this->m_empty == false)
    {
        for (int i = 0; i < this->getAllContains().size(); ++i)
        {
            if (typeid(*this->getAllContains()[i]) == typeid(Platform))
            {
                Platform* p = static_cast<Platform*>(this->getAllContains()[i]);

                if (p->getPos() == direction)
                {
                    return true;
                }
            }
        }
    }
    return false;
}
void Case::addPlatform(Platform p)
{
    this->m_empty = false;
    this->listPlatform.push_back(p);
    this->listDisplayable.push_back(&listPlatform.back());
}

void Case::addObstacle(Obstacle o)
{
    this->m_empty = false;
    this->listObstacle.push_back(o);
    this->listDisplayable.push_back(&listObstacle[listObstacle.size()-1]);
   
}
void Case::addPortal(Portal p)
{
    this->m_empty = false;
    this->portal = p;
    this->listDisplayable.push_back(&this->portal);
}
void Case::addDisplayable(Displayable* d)
{
    this->m_empty = false;
    this->listDisplayable.push_back(d);
}
std::vector<Displayable*> Case::getAllContains()
{
    return this->listDisplayable;
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
Level::Level(int n, int m) 
{
    this->n = n;
    this->m = m;

    /* Allocation dynamique */
    grid = new Case * [n];
    for (int i = 0; i < n; ++i)
    {
        grid[i] = new Case[m];
    }
    genereLevel();
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
void Level::set_platform(int i, int j, float rotation) const
{
    grid[i][j].addPlatform( Platform(i, j, Case::size_pixel_x, Case::size_pixel_y,rotation));
   
}
void Level::set_obstacle(int i, int j,bool traversable) const
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
            for (auto d : this->grid[columToCheck][j].getAllContains())
            {
                if (typeid(*d) == typeid(Platform))
                {
                    Platform* p = static_cast <Platform*>(d);
                    if ( p->getPos() == Platform::Position::TOP)
                    {
                        listDisplayable.push_back(static_cast <Platform*>(d));
                    }
                }
            }
        }
    }
    return listDisplayable;
}
std::vector<Displayable*> Level::getContainsCaseAt(int i, int j) const
{
    if (grid[i][j].isEmpty() == false) return grid[i][j].getAllContains();

    return std::vector<Displayable*>();
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
        this->set_platform(i, 0); // TOP BORDER
        this->set_platform(i, Level::grid_size-1);  //BOTTOM BORDER
        //this->set_platform(0, i, 90.0f); // LEFT BORDER
    }
    
    //this->set_platform(0, 0, 90.0f);
    this->set_platform(0, 1); this->set_platform(0, 3); this->set_platform(0, 4); this->set_platform(0, 6);
    this->set_platform(1, 2); this->set_platform(1, 3); this->set_platform(1, 5);
    this->set_platform(2, 1); this->set_platform(2, 3); this->set_platform(2, 4); this->set_platform(2, 6);
    this->set_platform(3, 2); this->set_platform(3, 4);  this->set_platform(3, 5);
    this->set_platform(4, 1); this->set_platform(4, 2); this->set_platform(4, 3); this->set_platform(4, 4); this->set_platform(4, 6);
    this->set_platform(5, 1); this->set_platform(5, 6);
    this->set_platform(6, 1); this->set_platform(6, 2); this->set_platform(6, 3); this->set_platform(6, 4); this->set_platform(6, 6);
    this->set_platform(7, 4); this->set_platform(7, 6);

    this->set_obstacle(1, 0,true);
    this->SetPortal(5, 5);
   
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


