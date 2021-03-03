#include <stdafx.h>

#include <Game/GameDemo.h>
#include <Game/GameLevel.h>

int main()
{
    //GameDemo game{};

    GameLevel game;
    game.RunGameLoop();


    return EXIT_SUCCESS;
}