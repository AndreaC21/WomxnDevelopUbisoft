#pragma once

#include <Engine/Game.h>
#include <Game/Player.h>
#include <Game/MainCharacter.h>
#include <Game/Platform.h>
#include <Game/Obstacle.h>
#include <Game/Ennemy.h>

class Case
{
public:
    Case();
    Case(const Case&);    
   
    static int size_pixel_x;
    static int size_pixel_y;
  
    void addPlatform(Platform p);
    void addObstacle(Obstacle o);
    void addDisplayable(Displayable* d);
  
    std::vector<Displayable*> getAllContains();
    bool isEmpty() const;

private:
    
    bool m_empty;
   
public:
    std::vector<Platform> listPlatform;
    std::vector<Obstacle> listObstacle;
    std::vector<Displayable*> listDisplayable;
   

};

class Level
{
public:
    Level();
    Level(const Level&);
    Level(int i, int j);
    ~Level();

    static int grid_size;

    Case at(int i, int j) const;

    void set(int i, int j, Displayable d) const;
    void set_platform(int i, int j, float rotation=0) const;
    void set_obstacle(int i, int j, bool) const;

    std::vector<Displayable*> getContainsCaseAt(int i, int j) const;

    int getSize_n() const;
    int getSize_m() const;

    std::vector<Platform*> getColumnsPlatform(int i);
   
   

private:
    Case** grid;
    int n,m;
    void genereLevel();
    void path_planning_a_star(Case start,Case target);
};

class GameLevel : public Game
{
public:
    GameLevel();

    void Update(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;

    void generateLevel();
    void generatePlatform();
    void generateEnnemy();

private:

    void buildListPlatform();

    static int max_size_x, max_size_y;
  //  Player m_player;
    Player m_Player;
    std::vector<Ennemy> m_listEnnemy;
   
    std::vector<Platform> list_platform;
    std::vector<Displayable*> list_displayable;
    Displayable m_Background;
    sf::View m_View;
    bool m_IsFinished;
    Level m_level;
    sf::Clock m_clock;

};
