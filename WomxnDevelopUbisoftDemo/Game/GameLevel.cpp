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
       
        if (m_player.IsColliding(list_displayable[i])) m_player.AdjustPosition(list_displayable[i]);
        else counter++;
    }
    if ( counter== list_displayable.size()) m_player.setGrounded(false);

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
        target.draw(list_displayable[i]);
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

    if (ImGui::CollapsingHeader("Main character position"))
    {
        const auto& mainCharCenterPos = m_player.GetCenter();

        ImGui::Text("X: %f", mainCharCenterPos.x);
        ImGui::Text("Y: %f", mainCharCenterPos.y);
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
        if (m_player.isGrounded())
        {
            ImGui::TextColored(ImVec4(0, 255.f, 0.f, 1.f), "PLAYER ON GROUND");
        }
        else
        {
            ImGui::TextColored(ImVec4(255.f, 0.f, 0.f, 1.f), "PLAYER NOT ON GROUND");
        }
       
    }
    if (ImGui::CollapsingHeader("Platform status"))
    {
       
        ImGui::Text("collision detected: %d", m_player.index_collision(list_displayable[0]));
        ImGui::Text("top: %f - top+width: %f",list_displayable[0].GetBoundingBox().top, (list_displayable[0].GetBoundingBox().top+ list_displayable[0].GetBoundingBox().width));
        ImGui::Text("top+height: %f", list_displayable[0].GetBoundingBox().top+ list_displayable[0].GetBoundingBox().height);
        ImGui::Text("left: %f", list_displayable[0].GetBoundingBox().left);
       
        ImGui::Text("player box top: %f - top+width:%f", m_player.GetBoundingBox().top, m_player.GetBoundingBox().top + m_player.GetBoundingBox().width);
        ImGui::Text("player box left: %f", m_player.GetBoundingBox().left);

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
               
                list_displayable.push_back(m_level.at(i, j).getContains());
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
    m_displayable = c.m_displayable;
}
void Case::setContains(Displayable d)
{
    this->m_empty = false;
    this->m_displayable = d;
}
Displayable Case::getContains()
{
    return this->m_displayable;
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
        for (int j = 0; j < n; ++j)
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
void Level::set(int i, int j, Displayable d) const
{
    grid[i][j].setContains(d);
}
void Level::set_platform(int i, int j) const
{
    grid[i][j].setContains(Platform(i, j, Case::size_pixel_x, Case::size_pixel_y));
}
int Level::getSize_n() const
{
    return this->n;
}
int Level::getSize_m() const
{
    return this->m;
}

void Level::genereLevel()
{
    if (n <= 0 || m <= 0) return;

    
    for (int i = 0; i < Level::grid_size; ++i)
    {
        //this->set_platform(i, 0); // TOP BORDER
        //this->set_platform(i, Level::grid_size - 1);  //BOTTOM BORDER
    }
    this->set_platform(0, 0);
    //this->set_platform(0, 1);
    
}
void Level::path_planning_a_star(Case start, Case target)
{

}

#pragma endregion


