#include <stdafx.h>
#include "GameLevel.h"

GameLevel::GameLevel() : Game{ "Level 1" },
m_Background({0,0},"Background.png"),
m_player()
{
    m_player = Player();
    generateLevel();
     generatePlatform();
    // list_platform.push_back(Platform({ 100,400 }));
	 m_IsFinished=false;
     
}

void GameLevel::Update(float deltaTime)
{
    m_player.Update(deltaTime);

    int maxPlatform = list_platform.size();
    int counter = 0;
    for (int i = 0; i < list_platform.size(); ++i)
    {
        if (m_player.IsColliding(list_platform[i])) m_player.setGrounded(true);
        else counter++;
    }
    if ( counter==maxPlatform) m_player.setGrounded(false);

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
    target.draw(m_p);
    
    for (int i = 0; i < list_platform.size(); ++i)
    {
        target.draw(list_platform[i]);
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
    int taille_windows_x = 3450;
    int defaultSize = 1000;
    // Bordure
    list_platform.push_back(Platform({ 0,700 }, taille_windows_x,0));

    list_platform.push_back(Platform({ 0,0 }, taille_windows_x,90));

    list_platform.push_back(Platform({ 400,400 }, defaultSize, 0));
    
}