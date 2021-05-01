#include <stdafx.h>
#include "GameLevel.h"


#pragma region GameLevel

int GameLevel::max_size_y = Level::grid_size * Case::size_pixel_y;
int GameLevel::max_size_x = Level::grid_size * Case::size_pixel_x;

GameLevel::GameLevel() : Game{ "Level 1" },
m_GameSpriteLoader(),
m_level(),
m_Background({ 0,0 }, "Background.png",7.0f),
m_Player()
{
    generateLevel();
    GenerateEnnemy();
    LoadUI();
    m_TimeToCompleteLevel = 5.0f * 60.0f;
    m_CurrentTimeLevel = m_TimeToCompleteLevel;
    m_IsFinished = false;
    m_ExecuteEndGame = false;

    m_clock.restart();
}
void GameLevel::LoadUI()
{
    m_TextFont.loadFromFile(".\\Assets\\Font\\arial.ttf");

    // TextTime
    m_CurrentTime.setFont(m_TextFont);
    m_CurrentTime.setCharacterSize(24);
    m_CurrentTime.setFillColor(sf::Color::Black);
    m_CurrentTime.setStyle(sf::Text::Bold);
    m_CurrentTimeBackground.setSize(sf::Vector2f(70,30));

    // Ghost TextTime
    m_CurrentGhostTime.setFont(m_TextFont);
    m_CurrentGhostTime.setCharacterSize(24);
    m_CurrentGhostTime.setFillColor(sf::Color::Red);
    m_CurrentGhostTime.setStyle(sf::Text::Bold);

    m_CurrentTimeGhostBackground.setSize(sf::Vector2f(270, 30));
}

void GameLevel::Update(float deltaTime)
{
    if (isGameFinish()==false)
    { 
        UpdateCollisionWithPlayer();
        UpdateCollisionWithEnnemy();
        UpdateCollisionWithWeapons();
       
        m_Player.Update(deltaTime);
        UpdateEnnemy(deltaTime);
  
        if (m_Player.IsGhostMode())
        {
            UpdateGhostTextTime();
            UpdateTextTime(false);
        }
        else
        {
            UpdateTextTime(true);
        }
        m_View.setCenter(m_Player.GetPosition());
        m_Window.setView(m_View);

    }
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
#pragma region Update 
void GameLevel::UpdateEnnemy(float deltaTime)
{
    for (int i = 0; i < m_listEnnemy.size(); ++i)
    {
        m_listEnnemy[i].Update(deltaTime);

        if (m_listEnnemy[i].ToDestroy())
        {
            m_listEnnemy.erase(std::find(m_listEnnemy.begin(), m_listEnnemy.end(), m_listEnnemy[i]));
        }
    }
}
void GameLevel::UpdateCollisionWithPlayer()
{
    Player& player = GetPlayer();
    player.ResetCollision();

    UpdateCollision<Player>(player);
   
    if (player.IsColliding(GetPortal()))
    {
        m_PlayerSucceed = true;
        m_IsFinished = true;
    }

}
void GameLevel::UpdateCollisionWithEnnemy()
{
    for (Ennemy& ennemy : GetEnnemies())
    {
        ennemy.ResetCollision();

        UpdateCollision<Ennemy>(ennemy);

        if (ennemy.IsColliding(GetPlayer()))
        {
            ennemy.OnCollide(GetPlayer());
        }
    }    
}
void GameLevel::UpdateCollisionWithWeapons()
{
    for (Weapon& weapon : GetPlayerWeapon())
    {
        weapon.ResetCollision();
        UpdateCollision<Weapon>(weapon);
        for (Ennemy& ennemy : GetEnnemies())
        {
            if (weapon.IsColliding(ennemy))
            {
                weapon.OnCollide(ennemy);
            }
        }
    }
}
template <typename T>
void GameLevel::UpdateCollision(T& character)
{
    character.ResetCollision();

    for (Platform& platform : GetPlatforms() )//m_level.GetPlatformAround(character.GetPosition()))
    {
        if (character.IsColliding(platform))
        {
            character.OnCollide(platform);
        }
    }
    for (Obstacle& obstacle : GetObstacles())
    {
        if (character.IsColliding(obstacle))
        {
            character.OnCollide(obstacle);
        }
    }
}

void GameLevel::UpdateTextTime(bool needUpdate)
{
    m_CurrentTimeLevel = m_TimeToCompleteLevel - m_clock.getElapsedTime().asSeconds() + GetPlayer().GetTimeSpendInGhost();
    m_CurrentTime.setPosition(m_View.getCenter().x + 430,m_View.getCenter().y - 380);
    m_CurrentTimeBackground.setPosition(m_CurrentTime.getPosition());
    if (needUpdate)
    {
        int time = static_cast<int>(m_CurrentTimeLevel);
        int second = time % 60;
        int minute = (time / 60) % 60;
        std::string extraZero = (second < 10) ? "0" : "";
        m_CurrentTime.setString("0" + std::to_string(minute) + ":" + extraZero + std::to_string(second));
    }  
}
void GameLevel::UpdateGhostTextTime()
{
    m_CurrentTimeGhostBackground.setPosition(m_View.getCenter().x, m_View.getCenter().y - 380);
    m_CurrentGhostTime.setPosition(m_CurrentTimeGhostBackground.getPosition());

    m_CurrentGhostTime.setString("Return to body in 0" + std::to_string(GetPlayer().GetGhostCurrentTime())+"s");
}
#pragma endregion
void GameLevel::Render(sf::RenderTarget& target)
{
    target.clear(sf::Color(0, 0, 0));
    target.draw(m_Background);
   
    target.draw(m_CurrentTimeBackground);
    target.draw(m_CurrentTime);
    target.draw(m_Player);
   
    
    for (int i = 0; i < GetDisplayables().size(); ++i)
    {
        target.draw(*GetDisplayables()[i]);
    }
    if (m_Player.IsGhostMode() == false)
    {
        for (int i = 0; i < GetPlayerWeapon().size(); ++i)
        {
            target.draw(GetPlayerWeapon()[i]);
        }
    }
    else
    {
        target.draw(m_CurrentTimeGhostBackground);
        target.draw(m_CurrentGhostTime);
    }
    
    for (int i = 0; i < GetEnnemies().size(); ++i)
    {
        target.draw(GetEnnemies()[i]);
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

    if (ImGui::CollapsingHeader("Weapon"))
    {
        ImGui::Text("%d ", (int)GetPlayerWeapon().size()); 
    }
    if (ImGui::CollapsingHeader("Player"))
    {
       ImGui::Text("%s", m_Player.GetLifePoint().c_str());
       ImGui::Text("Position: %f , %f ", m_Player.GetPosition().x, m_Player.GetPosition().y);
       int c = m_level.GetIndexByPosition(m_Player.GetPosition()).first;
       int r = m_level.GetIndexByPosition(m_Player.GetPosition()).second;
       ImGui::Text("Case: %d , %d ", c, r);
       ImGui::Text("%d", (int) m_level.GetPlatformAround(m_Player.GetPosition()).size());
    }
    ImGui::End();
}

void GameLevel::generateLevel()
{
    m_View.reset(sf::FloatRect(0, 0, 1024, 768));
    m_Window.setView(m_View);
}
void GameLevel::GenerateEnnemy()
{
    m_listEnnemy.push_back(Ennemy(0,0, Case::size_pixel_x, Case::size_pixel_y,&m_Player));
    m_listEnnemy.push_back(Ennemy(1,2, Case::size_pixel_x, Case::size_pixel_y, &m_Player));
    m_listEnnemy.push_back(Ennemy(2,2, Case::size_pixel_x, Case::size_pixel_y, &m_Player));
    m_listEnnemy.push_back(Ennemy(3,3, Case::size_pixel_x, Case::size_pixel_y, &m_Player));
    m_listEnnemy.push_back(Ennemy(5, 0, Case::size_pixel_x, Case::size_pixel_y, &m_Player));

}

bool GameLevel::isGameFinish()
{
    if (m_Player.IsGhostMode() == false)
    {
        if (m_Player.IsDead())
        {
            m_PlayerSucceed = false;
            m_IsFinished = true;
        }
    }
    if (m_CurrentTimeLevel <= 0.0f)
    {
        m_PlayerSucceed = false;
        m_IsFinished = true;
    }
    return m_IsFinished;
}
void GameLevel::StartEndGameSuccess()
{
    m_EndGameTextTime.setFont(m_TextFont);
    m_EndGameTextTime.setCharacterSize(24);
    m_EndGameTextTime.setFillColor(sf::Color::Red);
    m_EndGameTextTime.setStyle(sf::Text::Bold);
    m_EndGameTextTime.setPosition(m_Player.GetPosition().x, m_Player.GetPosition().y -250);

    m_EngameTexture.loadFromFile(".\\Assets\\EndGame\\Success.png");

    m_EndGameTime = m_clock.getElapsedTime().asSeconds();
    m_EndGameTextTime.setString("Time to complete level: " + std::to_string(m_EndGameTime));
    m_EndGameSprite.setTexture(m_EngameTexture);
    m_EndGameSprite.setPosition(m_Player.GetPosition().x, m_Player.GetPosition().y - 100);
}
void GameLevel::StartEndGameFail()
{
    m_EngameTexture.loadFromFile(".\\Assets\\EndGame\\Fail.png");
    m_EndGameSprite.setTexture(m_EngameTexture);
    m_EndGameSprite.setPosition(m_Player.GetPosition().x, m_Player.GetPosition().y -100);
}
#pragma region GETTER
Player& GameLevel::GetPlayer()
{
    return m_Player;
}
Portal& GameLevel::GetPortal()
{
    return m_level.GetPortal();
}
std::vector<Ennemy>& GameLevel::GetEnnemies()
{
    return m_listEnnemy;
}
std::vector<Weapon>& GameLevel::GetPlayerWeapon()
{
    return GetPlayer().GetWeapons();
}
std::vector<Displayable*>& GameLevel::GetDisplayables()
{
    return m_level.GetAllDisplayable();
}
std::vector<Platform>& GameLevel::GetPlatforms()
{
    return m_level.GetPlatforms();
}
std::vector<Obstacle>& GameLevel::GetObstacles()
{
    return m_level.GetObstacles();
}
#pragma endregion

#pragma endregion

#pragma region Case

int Case::size_pixel_x = 600;
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
        for (int i = 0; i < c.m_Obstacles.size(); ++i)
        {
            m_Obstacles.push_back(c.m_Obstacles[i]);
        }
        for (int i = 0; i < c.m_Platforms.size(); ++i)
        {
            m_Platforms.push_back(c.m_Platforms[i]);
        }
        this->m_Portal = c.m_Portal;
    }  
}

Portal& Case::GetPortal() { return this->m_Portal; }

std::vector<Platform>& Case::GetPlatforms() 
{ 
    return this->m_Platforms;
}
std::vector<Obstacle>& Case::GetObstacles() 
{
    return this->m_Obstacles;
}

bool Case::HasObstacle(Platform::ePosition direction)
{
    if (this->m_empty == false)
    {
        for (int i = 0; i < this->GetPlatforms().size(); ++i)
        {
            if (this->GetPlatforms()[i].getPos() == direction)
            {
                return true;
            } 
        }
    }
    return false;
}
void Case::AddContent(Platform p)
{
    this->m_empty = false;
    this->m_Platforms.push_back(p);
}

void Case::AddContent(Obstacle o)
{
    this->m_empty = false;
    this->m_Obstacles.push_back(o); 
}
void Case::AddContent(Portal p)
{
    this->m_empty = false;
    this->m_Portal = p;
}

bool Case::IsEmpty() const
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
    this->n = l.GetSize_n();
    this->m = l.GetSize_m();
    grid = new Case * [this->n];
    for (int i = 0; i < n; ++i)
    {
        grid[i] = new Case[this->m];
        for (int j = 0; j < m; ++j)
        {
            this->grid[i][j] = l.At(i, j);
        }
    }
}

Case& Level::At(int i, int j) const
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
std::vector<Displayable*>& Level::GetAllDisplayable()
{
    return m_listDisplayable;
}
std::pair<int, int> Level::GetIndexByPosition(sf::Vector2f position) const
{
    int i = static_cast<int>(std::floor(position.x / Case::size_pixel_x));
    int j = static_cast<int>(std::floor(position.y / Case::size_pixel_y));

    return std::pair<int,int>(i,j);
}
void Level::SetPlatform(int i, int j, float rotation) const
{
    grid[i][j].AddContent(Platform(i, j, Case::size_pixel_x, Case::size_pixel_y,rotation));
}
void Level::SetObstacle(int i, int j,bool traversable) const
{
    grid[i][j].AddContent(Obstacle(i, j, Case::size_pixel_x, Case::size_pixel_y,traversable));
}
void Level::SetPortal(int i, int j)
{
    grid[i][j].AddContent(Portal(i, j, Case::size_pixel_x, Case::size_pixel_y));
    m_Portal = grid[i][j].GetPortal();
}
int Level::GetSize_n() const
{
    return this->n;
}
int Level::GetSize_m() const
{
    return this->m;
}
std::vector<Platform> Level::GetColumnsPlatform(int columToCheck,int rowToBegin )
{
    std::vector<Platform> result;
    for (int j = rowToBegin+1; j < GetSize_m(); ++j)
    {
        if (this->grid[columToCheck][j].IsEmpty() == false)
        { 
            for (Platform& p : this->grid[columToCheck][j].GetPlatforms())
            {
                if ( p.getPos() == Platform::ePosition::TOP)
                {
                    result.push_back(p);
                }
            }
        }
    }
    return result;
}
std::vector<Platform> Level::GetPlatformsByColumn(const int column,int row)const 
{
    std::vector<Platform> result;

    if (row > 0)
    {
        for (Platform& p : this->At(column, row-1).GetPlatforms())
        {
            result.push_back(p);
        }
    }
    for (Platform& p : this->At(column,row).GetPlatforms())
    {
        result.push_back(p);
    }
    if (row < Level::grid_size)
    {
        for (Platform& p : this->At(column, row+1).GetPlatforms())
        {
            result.push_back(p);
        }
    }

    return result;
}
std::vector<Platform> Level::GetPlatformAround(sf::Vector2f position) const
{
    std::vector<Platform> result;
    if (position.x < 0 || position.y < 0)
    {
        return result;
    }
    std::pair<int, int> indexes = GetIndexByPosition(position);
    int column = indexes.first;
    int row = indexes.second;

    if (column > 0)
    {
        for (Platform& p : GetPlatformsByColumn(column-1,row))
        {
            result.push_back(p);
        }     
    }
    for (Platform& p : GetPlatformsByColumn(column, row))
    {
        result.push_back(p);
    }
    if (column < Level::grid_size)
    {
        for (Platform& p : GetPlatformsByColumn(column + 1, row))
        {
            result.push_back(p);
        }
    }

    return result;
}
std::vector<Platform>& Level::GetPlatforms()
{
    return m_Platforms;
}
std::vector<Obstacle>& Level::GetObstacles()
{
    return m_Obstacle;
}
Portal& Level::GetPortal()
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
    
  //  this->SetObstacle(1, 0, true);

    // Horizontal
    this->SetPlatform(0, 1); this->SetPlatform(0, 3); this->SetPlatform(0, 4); this->SetPlatform(0, 6);
    this->SetPlatform(1, 2); this->SetPlatform(1, 3); this->SetPlatform(1, 5);
    this->SetPlatform(2, 1); this->SetPlatform(2, 3); this->SetPlatform(2, 4); this->SetPlatform(2, 6);
    this->SetPlatform(3, 2); this->SetPlatform(3, 4); this->SetPlatform(3, 5);
    this->SetPlatform(4, 1); this->SetPlatform(4, 2); this->SetPlatform(4, 3); this->SetPlatform(4, 4); this->SetPlatform(4, 6);
    this->SetPlatform(5, 1); this->SetPlatform(5, 3); this->SetPlatform(5, 4); this->SetPlatform(5, 5); this->SetPlatform(5, 6);
    this->SetPlatform(6, 1); this->SetPlatform(6, 2); this->SetPlatform(6, 3); this->SetPlatform(6, 4);

    //Vertical
    this->SetPlatform(1,4, 90.0f);
    this->SetPlatform(2,5, 90.0f);
    this->SetPlatform(3, 0, 90.0f); this->SetPlatform(3, 4, 90.0f);
    this->SetPlatform(4, 6, 90.0f);
    this->SetPlatform(5, 2, 90.0f);
    this->SetPlatform(6, 3, 90.0f);
    
    this->SetPortal(5, 5);

    BuildListDisplayable();
}

void Level::BuildListDisplayable()
{
    for (int i = 0; i < this->n; ++i)
    {
        for (int j = 0; j < this->m; ++j)
        {
            if (this->At(i, j).IsEmpty() == false)
            {
                for (int h = 0; h < this->At(i, j).GetPlatforms().size(); ++h)
                {
                    m_listDisplayable.push_back(&(grid[i][j].GetPlatforms()[h]));
                    m_Platforms.push_back(grid[i][j].GetPlatforms()[h]);
                }
                for (int h = 0; h < this->At(i, j).GetObstacles().size(); ++h)
                {
                    m_listDisplayable.push_back(&(grid[i][j].GetObstacles()[h]));
                    m_Obstacle.push_back(grid[i][j].GetObstacles()[h]);
                }
            }
        }
    }
    m_listDisplayable.push_back(&m_Portal);
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
    return Case();    
}


#pragma endregion


