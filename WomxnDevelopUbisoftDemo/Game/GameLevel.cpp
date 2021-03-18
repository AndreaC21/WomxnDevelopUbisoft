#include <stdafx.h>
#include "GameLevel.h"

#pragma region GameLevel

int GameLevel::max_size_y = Level::grid_size * Case::size_pixel_y;
int GameLevel::max_size_x = Level::grid_size * Case::size_pixel_x;

GameLevel::GameLevel() : Game{ "Level 1" },
m_Background({ 0,0 }, "Background.png",3.5f),
m_player(),
m_level()
{
     generateLevel();
     
     buildListPlatform();
	 m_IsFinished=false;
     
}

void GameLevel::Update(float deltaTime)
{
    m_player.Update(deltaTime);
    int counter = 0;
    
   
    for (int i = 0; i < list_displayable.size(); ++i)
    {
        if (m_player.IsColliding(*list_displayable[i]))
        {
          m_player.AdjustPosition(list_displayable[i]);
        }
        else
        {
            counter++;
        }
    }
    if (counter == list_displayable.size())
    {
        m_player.setGrounded(false);
        m_player.m_blockLeftRight = false;
    }

    for (int i = 0; i < m_player.getWeapon().size(); ++i)
    {
        m_player.getWeapon()[i].Update(deltaTime);
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
    
    m_View.setCenter(m_player.getPosition());
    m_Window.setView(m_View);
}

void GameLevel::Render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));
    target.draw(m_Background);

    target.draw(m_player);
    
    for (int i = 0; i < list_displayable.size(); ++i)
    {
        target.draw(*list_displayable[i]);
    }
    for (int i = 0; i < m_player.getWeapon().size(); ++i)
    {
        target.draw(m_player.getWeapon()[i]);
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
        
        ImGui::Text("X: %f Y: %f", m_player.getPosition().x, m_player.getPosition().y);
        ImGui::Text("LF: %s", m_player.getLifePoint());
        ImGui::Text("LF: %f", m_player.getCurrentLifePoint());
    }

    if (ImGui::CollapsingHeader("Game status"))
    {
        if (m_IsFinished)
        {
            ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "GAME ENDED");
        }
        else
        {
            ImGui::TextColored(ImVec4(0.f, 255.0f, 0.f, 1.f), "GAME IN PROGRESS");
        }
    }

    if (ImGui::CollapsingHeader("Colliding status"))
    {
       
        if (m_player.isGhostMode())
        {
            ImGui::TextColored(ImVec4(0, 255.f, 0.f, 1.f), "Ghost");
        }
        else
        {
            ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "Explorator");
        }
       
    }
    if (ImGui::CollapsingHeader("Platform status"))
    {
       
    }
    if (ImGui::CollapsingHeader("Weapon status"))
    {
        ImGui::Text("Number weapon: %d",(int) m_player.getWeapon().size());

        if (m_player.getWeapon().size() > 0)
        {
            ImGui::Text("X: %f Y: %f", (int)m_player.getWeapon()[0].getPosition().x, (int)m_player.getWeapon()[0].getPosition().y);
            
            if (m_player.getWeapon()[0].Finish()==false)
            { 
                ImGui::TextColored(ImVec4(0, 255.f, 0.f, 1.f), "Alive");
            }
            else
            {
                ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "Dead");
            }
        }
      
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
void GameLevel::buildListPlatform()
{
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


