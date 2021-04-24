#pragma once

#include <Engine/Game.h>
#include <Game/Player.h>
#include <Game/Platform.h>
#include <Game/Obstacle.h>
#include <Game/Ennemy.h>
#include <Game/Portal.h>

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
    bool HasObstacle(Platform::ePosition direction);

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

    Case at(int i, int j) const;

    void SetPlatform(int i, int j, float rotation=0.0f) const;
    void SetObstacle(int i, int j, bool) const;
    void SetPortal(int i, int j);

    std::vector<Displayable*>& GetAllDisplayable();
    int GetSize_n() const;
    int GetSize_m() const;
    Portal* getPortal() const;
    std::vector<Platform> GetColumnsPlatform(int column,int rowToBegin);
    std::vector<Platform> GetPlatformAround(int column, int row) const;
    std::vector<Platform>& GetPlatforms();
    std::vector<Obstacle>& GetObstacles();


    void BuildListDisplayable();

    struct Direction {
        int x, y;

        Direction() { x = -1; y = -1; }
        Direction(int x, int y)
        {
            this->x = x;
            this->y = y;
        }
        int getX() const { return this->x; }
        int getY() const { return this->y; }

        
        inline bool operator==(const Direction& d)
        {
            return true;
          //  return (this->x == d.getX() && this->y = d.getY());
        }

    };

private:

    Case** grid;
    int n,m;
    Portal* m_Portal;
    std::vector<Displayable*> m_listDisplayable;
    std::vector<Platform> m_Platforms;
    std::vector<Obstacle> m_Obstacle;

    std::vector<std::pair<Case,Direction>> open, closed;
    std::vector<Case> resultJumpPointSearch;

    void genereLevel();

    Case jumpPointSearch();
    Case searchHorizontale(Case pos, int horizontaleDirection, float distance,Case Destination);
    Case searchDiagoanle();  
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
    void UpdateCollision(std::vector<Character>);

    Player& GetPlayer();
    std::vector<Ennemy>& GetEnnemies();
    std::vector<Weapon>& GetPlayerWeapon();
    std::vector<Platform>& GetPlatforms();
    std::vector<Obstacle>& GetObstacles();
    std::vector<Displayable*>& GetDisplayables();

private:

    static int max_size_x, max_size_y;
    Player m_Player;
    std::vector<Ennemy> m_listEnnemy;
    Displayable m_Background;
    sf::View m_View;
    bool m_IsFinished;
    Level m_level;
    sf::Clock m_clock;

    bool m_ExecuteEndGame,m_PlayerSucceed;
    sf::Font m_TextFont;
    float m_EndGameTime;
    sf::Text m_EndGameTextTime;
    sf::Texture m_EngameTexture;
    sf::Sprite m_EndGameSprite;
};
