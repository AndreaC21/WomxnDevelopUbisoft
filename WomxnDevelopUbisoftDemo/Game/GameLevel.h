#pragma once

#include <Engine/Game.h>
#include <Game/Player.h>
#include <Game/MainCharacter.h>
#include <Game/Platform.h>
#include <Game/Obstacle.h>



class Case
{
public:
    Case();
    Case(const Case&);    
   
    static int size_pixel_x;
    static int size_pixel_y;
  
    void addContains(Displayable d);
  
    std::vector<Displayable> getAllContains();
    bool isEmpty() const;

private:
    
    bool m_empty;
   
public:
    std::vector<Displayable> listDisplayable;
   

};

class Level
{
public:
    Level();
    Level(const Level&);
    Level(int i, int j);

    static int grid_size;

    Case at(int i, int j) const;

    void set(int i, int j, Displayable d) const;
    void set_platform(int i, int j, float rotation=0) const;
    void set_obstacle(int i, int j) const;

    int getSize_n() const;
    int getSize_m() const;
   
   

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

private:

    void buildListPlatform();

    static int max_size_x, max_size_y;
    Player m_player;
   
    std::vector<Platform> list_platform;
    //std::vector<Displayable> list_displayable;
    std::vector<Displayable*> list_displayable;
    Displayable m_Background;
    sf::View m_View;
    bool m_IsFinished;
    Level m_level;

};
