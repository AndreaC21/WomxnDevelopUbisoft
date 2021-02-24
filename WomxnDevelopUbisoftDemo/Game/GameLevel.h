#pragma once

#include <Engine/Game.h>
#include <Game/Player.h>

class GameLevel : public Game
{
public:
    GameLevel();

    void Update(float deltaTime) override;
    void Render(sf::RenderTarget& target) override;
    void RenderDebugMenu(sf::RenderTarget& target) override;

private:
   
    Player m_player;
    bool m_IsFinished;
};