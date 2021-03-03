#pragma once

#include <Engine/Game.h>
#include <Game/Player.h>
#include <Game/MainCharacter.h>
#include <Game/Platform.h>

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
   
    Player m_player;
    Platform m_p;
    std::vector<Platform> list_platform;
    Displayable m_Background;
    sf::View m_View;
    bool m_IsFinished;
   
};