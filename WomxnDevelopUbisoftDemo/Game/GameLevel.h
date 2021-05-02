#pragma once

#include <Engine/Game.h>
#include <Engine/PathFinding.h>
#include <Game/Player.h>
#include <Game/Platform.h>
#include <Game/Obstacle.h>
#include <Game/Ennemy.h>
#include <Game/Portal.h>
#include <Engine/GameSpriteLoader.h>

class Case
{
public:
    Case();
    Case(const Case&);    
   
    static int size_pixel_x;
    static int size_pixel_y;
  
    void AddContent(Portal p );
    void AddContent(Platform p);
    void AddContent(Obstacle o);
  
    std::vector<Platform>& GetPlatforms();
    std::vector<Obstacle>& GetObstacles();
    Portal& GetPortal();
    bool IsEmpty() const;
    bool HasPlatform(Platform::ePosition direction);

private:
    bool m_empty;
    Portal m_Portal;
    std::vector<Platform> m_Platforms;
    std::vector<Obstacle> m_Obstacles;
    
public:
    inline bool operator==(const Case& c)
    {
        return true;
    }
};

class Level
{
public:
    Level();
    Level(const Level&);
    ~Level();

    static int grid_size;

    Case& At(int i, int j) const;

    void SetPlatform(int i, int j, float rotation=0.0f) const;
    void SetObstacle(int i, int j, bool) const;
    void SetPortal(int i, int j);

    std::vector<Displayable*>& GetAllDisplayable();
    int GetSize_n() const;
    int GetSize_m() const;
    std::pair<int, int> GetIndexByPosition(sf::Vector2f position) const;
    Portal& GetPortal();
    std::vector<Platform> GetColumnsPlatform(int column,int rowToBegin);
    std::vector<Platform> GetPlatformAround(sf::Vector2f) const;
    std::vector<Platform>& GetPlatforms();
    std::vector<Platform> GetPlatformsByColumn(const int column,int row) const;
    std::vector<Obstacle>& GetObstacles();

    void BuildListDisplayable();

private:

    Case** grid;
    int n,m;
    Portal m_Portal;
    std::vector<Displayable*> m_listDisplayable;
    std::vector<Platform> m_Platforms;
    std::vector<Obstacle> m_Obstacle;

    
    void genereLevel();

};

class GameLevel : public Game
{
public:
    GameLevel();

    void Update(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;

    void generateLevel();
    void GenerateEnnemy();

    bool isGameFinish();
    void StartEndGameSuccess();
    void StartEndGameFail();

    void UpdateEnnemy(float deltatime);
    
    void UpdateCollisionWithPlayer();
    void UpdateCollisionWithEnnemy();
    void UpdateCollisionWithWeapons();
    template <typename T>
    void UpdateCollision(T&);

    Player& GetPlayer();
    Portal& GetPortal();
    std::vector<Ennemy>& GetEnnemies();
    std::vector<Weapon>& GetPlayerWeapon();
    std::vector<Platform>& GetPlatforms();
    std::vector<Obstacle>& GetObstacles();
    std::vector<Displayable*>& GetDisplayables();

private:

    static int max_size_x, max_size_y;
    GameSpriteLoader m_GameSpriteLoader;
    Player m_Player;
    std::vector<Ennemy> m_listEnnemy;
    Displayable m_Background;
    sf::View m_View;
    bool m_IsFinished;
    Level m_level;
    sf::Clock m_clock;
    float m_TimeToCompleteLevel,m_CurrentTimeLevel;

    bool m_ExecuteEndGame,m_PlayerSucceed;
    sf::Font m_TextFont;
    float m_EndGameTime;
    sf::Text m_EndGameTextTime;
    sf::Texture m_EngameTexture;
    sf::Sprite m_EndGameSprite;

    sf::Text m_CurrentTime;
    sf::RectangleShape m_CurrentTimeBackground;
    sf::Text m_CurrentGhostTime;
    sf::RectangleShape m_CurrentTimeGhostBackground;

    //PathFinding m_JPS;
    
  
    void LoadUI();
    void UpdateTextTime(bool);
    void UpdateGhostTextTime();
};
