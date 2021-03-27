#include <stdafx.h>
#include "GameLevel.h"

bool b = false;
#pragma region GameLevel

int GameLevel::max_size_y = Level::grid_size * Case::size_pixel_y;
int GameLevel::max_size_x = Level::grid_size * Case::size_pixel_x;

GameLevel::GameLevel() : Game{ "Level 1" },
m_Background({ 0,0 }, "Background.png",3.5f),
m_Player(),
m_level()
{
    generateLevel();
    generateEnnemy();

    buildListPlatform();
    m_IsFinished = false;
    m_clock.restart();

    //m_listEnnemy.push_back(Ennemy(sf::Vector2f{ 400, 150 }, &m_player));
   // m_listEnnemy.push_back(Ennemy(sf::Vector2f{ 400, 150 }));
     
}

void GameLevel::Update(float deltaTime)
{
   
    int counter = 0;
   // m_player.UpdateShoot(m_clock.getElapsedTime());
   
    for (int i = 0; i < list_displayable.size(); ++i)
    {
        if (m_Player.ptr->IsColliding(*list_displayable[i]))
        {
          m_Player.AdjustPosition(list_displayable[i]);
        }
        else
        {
            counter++;
        }
        
        if (m_Player.isGhostMode()==false)
        {
            Explorator* o = static_cast <Explorator*>(m_Player.ptr);
            o->UpdateWeapon(list_displayable[i]);
        }
       
    }
    if (m_Player.isGhostMode() == false)
    {
        Explorator* o = static_cast <Explorator*>(m_Player.ptr);
        o->UpdateShoot(m_clock.getElapsedTime());
    }
    
    m_Player.Update(deltaTime); 
    
    for (int i =0; i < m_listEnnemy.size() ;++i)
    {
        m_listEnnemy[i].Update(deltaTime);
        if (m_Player.isGhostMode() == false)
        {
            Explorator* o = static_cast <Explorator*>(m_Player.ptr);
            o->UpdateWeapon(m_listEnnemy[i]);
        }
      
        if (m_listEnnemy[i].isGrounded() == false)
        {
            m_listEnnemy[i].Fall();

            for (auto& p : this->m_level.getColumnsPlatform(m_listEnnemy[i].getSpawnedColumns()))
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
    /*
    for (int i = 0; i < m_player.getWeapon().size(); ++i)
    {
        m_player.getWeapon()[i].Update(deltaTime);
    }
    */
  
    if (counter == list_displayable.size())
    {
        m_Player.NoCollisionDetected();
        //m_player.setGrounded(false);
       // m_player.m_blockLeftRight = false;
    }

   
    if (!m_IsFinished)
    {
        //if (m_Door.IsColliding(m_MainCharacter))
        //if (m_Door.Contains(m_MainCharacter.GetCenter()))
        /*if (m_Door.Contains(m_player))
        {

            m_MainCharacter.StartEndGame();
            m_Door.StartEndGame();
            m_IsFinished = true;
        }*/
    }
    
    m_View.setCenter(m_Player.getPosition());
    m_Window.setView(m_View);
}

void GameLevel::Render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));
    target.draw(m_Background);

   // target.draw(m_player);
    target.draw(*m_Player.ptr);
    
    for (int i = 0; i < list_displayable.size(); ++i)
    {
        target.draw(*list_displayable[i]);
    }
    if (m_Player.isGhostMode() == false)
    {
        Explorator* e = static_cast <Explorator*>(m_Player.ptr);

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
        ImGui::Text("type: %s", typeid(*m_Player.ptr).name());
       // ImGui::Text("LF: %s", m_player.getLifePoint().c_str());
       // ImGui::Text("LF: %f", m_player.getCurrentLifePoint());
    }

 
    if (ImGui::CollapsingHeader("Colliding status"))
    {
        ImGui::Text("index collision: %d ", m_Player.AdjustPosition(list_displayable[1]));

        ImGui::Text("up:%d down:%d left:%d right:%d", m_Player.ptr->m_BlockDirection[0], m_Player.ptr->m_BlockDirection[1], m_Player.ptr->m_BlockDirection[2], m_Player.ptr->m_BlockDirection[3]);
       
       
    }
   
  
    if (ImGui::CollapsingHeader("Weapon status"))
    {
        if (typeid(*m_Player.ptr) == typeid(Explorator))
        {
            Explorator* e = static_cast <Explorator*>(m_Player.ptr);
            ImGui::Text("Number weapon: %d", (int)e->getWeapon().size());
            
        }
   
    }
    if (ImGui::CollapsingHeader("Ennemy status"))
    {
        ImGui::Text("Number ennemy: %d", (int)m_listEnnemy.size());
        ImGui::Text("PV: %f", m_listEnnemy[0].getCurrentLifePoint());
        ImGui::Text("X: %f Y: %f", m_listEnnemy[0].getPosition().x, m_listEnnemy[0].getPosition().y);
       
       
    }
   
    ImGui::End();
}

void GameLevel::generateLevel()
{
    m_View.reset(sf::FloatRect(0, 0, 1024, 768));
   
    m_Window.setView(m_View);
   
    //m_p = Platform({ 500,500 }, 1000);
    //m_Background = Displayable({ 0,0 }, "Player\\Idle_1.png");
}
void GameLevel::generatePlatform()
{
  
}
void GameLevel::generateEnnemy()
{
    m_listEnnemy.push_back(Ennemy(0,0, Case::size_pixel_x, Case::size_pixel_y));

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

#pragma endregion

#pragma region Case

int Case::size_pixel_x = 1000;
int Case::size_pixel_y = 300;

Case::Case()
{
    m_empty = true;
}
Case::Case(const Case& c)
{
    m_empty = c.m_empty;
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
    }
   
}

void Case::addPlatform(Platform p)
{
    this->m_empty = false;
    this->listPlatform.push_back(p);
    this->listDisplayable.push_back(&listPlatform[listPlatform.size() - 1]);
}

void Case::addObstacle(Obstacle o)
{
    this->m_empty = false;
    this->listObstacle.push_back(o);
    this->listDisplayable.push_back(&listObstacle[listObstacle.size()-1]);
   
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
int Level::getSize_n() const
{
    return this->n;
}
int Level::getSize_m() const
{
    return this->m;
}
std::vector<Platform*> Level::getColumnsPlatform(int i)
{
    std::vector<Platform*> listDisplayable;
    for (int j = 0; j < getSize_m(); ++j)
    {
        if (this->grid[i][j].isEmpty() == false)
        {
            for (auto& d : this->grid[i][j].getAllContains())
            {
                if (typeid(*d) == typeid(Platform))
                {
                    Platform* p = static_cast <Platform*>(d);
                   // if (p->getPos() == Platform::Position::BOTTOM || p->getPos() == Platform::Position::TOP)
                   // {
                        listDisplayable.push_back(static_cast <Platform*>(d));
                    //}
                }
            }
        }
    }
    return listDisplayable;
}
std::vector<Displayable*> Level::getContainsCaseAt(int i, int j) const
{
    if (grid[i][j].isEmpty() == false) return  grid[i][j].getAllContains();

    return std::vector<Displayable*>();
}

void Level::genereLevel()
{
    if (n <= 0 || m <= 0) return;

    
    for (int i = 0; i < Level::grid_size; ++i)
    {
        //this->set_platform(i, 0); // TOP BORDER
        //this->set_platform(i, Level::grid_size-1);  //BOTTOM BORDER
        //this->set_platform(0, i, 90.0f); // LEFT BORDER
    }
    
    
    this->set_platform(0, 1); //this->set_platform(2, 1);
   /* this->set_platform(3, 0, 90);
   
    this->set_platform(1, 2);
    this->set_platform(0, 3); this->set_platform(1, 3); this->set_platform(2, 3);
    this->set_platform(2, 4); this->set_platform(3, 4);
    */
    this->set_obstacle(1, 0,true);
   
}
void Level::path_planning_a_star(Case start, Case target)
{

}


#pragma endregion


