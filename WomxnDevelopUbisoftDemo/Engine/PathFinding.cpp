#include "stdafx.h"
#include "PathFinding.h"
#include <Game/GameLevel.h>

PathFinding::PathFinding()
{

}
void PathFinding::Start(Level& level, std::pair<int, int> from, std::pair<int, int> target)
{
	//this->m_Level = level;
	this->m_Start = from;
	this->m_end = target;

    JPS(this->m_Start);

}
std::vector<std::pair<int, int>> PathFinding::GetResultJPS() const
{
    return m_ResultJPS;
}

std::vector<std::pair<int, int>> PathFinding::AvailableNextCase(int currentColumn, int currentRow)
{
	//std::vector<Case&> result;
    std::vector<std::pair<int, int>> results;
    /*
    if (currentColumn > 0)
    {
        // UPPER LEFT
        if (m_Level.At(currentColumn, currentRow).HasPlatform(Platform::ePosition::RIGHT) == false)
        {
            results.push_back(std::pair<int, int>(currentColumn - 1, currentRow - 1));
        }
        //MIDDLE LEFT
        if (m_Level.At(currentColumn, currentRow).HasPlatform(Platform::ePosition::RIGHT) == false)
        {
            results.push_back(std::pair<int, int>(currentColumn - 1, currentRow));
        }
        // LOWER LEFT
        if (m_Level.At(currentColumn - 1, currentRow + 1).HasPlatform(Platform::ePosition::TOP)==true)
        {
            results.push_back(std::pair<int, int>(currentColumn - 1, currentRow + 1));
        }
    }
    // UPPER
    if (m_Level.At(currentColumn, currentRow).HasPlatform(Platform::ePosition::TOP)==false)
    {
        results.push_back(std::pair<int, int>(currentColumn, currentRow - 1));
    }
    // LOWER
    if (m_Level.At(currentColumn, currentRow+1).HasPlatform(Platform::ePosition::TOP) == false)
    {
        //result.push_back(m_Level.At(currentColumn, currentRow+1));
        results.push_back(std::pair<int, int>(currentColumn, currentRow + 1));
    }

    if (currentColumn < Level::grid_size)
    {
        // UPPER RIGHT
        if (m_Level.At(currentColumn+1, currentRow -1).HasPlatform(Platform::ePosition::LEFT) == false)
        {
            results.push_back(std::pair<int, int>(currentColumn + 1, currentRow - 1));
        }
        //MIDDLE RIGHT
        if (m_Level.At(currentColumn + 1, currentRow).HasPlatform(Platform::ePosition::LEFT) == false)
        {
           results.push_back(std::pair<int, int>(currentColumn + 1, currentRow));
        }
        // LOWER RIGHT
        if (m_Level.At(currentColumn + 1, currentRow + 1).HasPlatform(Platform::ePosition::TOP)==false)
        {
            results.push_back(std::pair<int, int>(currentColumn + 1, currentRow + 1));
        }
    }
   */
	return results;
}

float EuclideanDistance(std::pair<int, int> a,std::pair<int, int> b)
{
    return (float)sqrt(pow(a.first - b.first, 2) + pow(a.second - b.second, 2));
}
std::pair<int, int> PathFinding::GetBestOption(std::pair<int, int> currentCase,std::vector<std::pair<int, int>> availableCase)
{
    std::pair<int, int> bestCase = availableCase[0];
    float ed = EuclideanDistance(currentCase,availableCase[0]);
    for (std::pair<int, int> c : availableCase)
    {
        if ( EuclideanDistance(currentCase, c))
        {
            ed = EuclideanDistance(currentCase, c);
            bestCase = c;
        }
    }
    return bestCase;
}

void PathFinding::JPS(std::pair<int, int> current)
{
    if (current != m_end)
    {
        std::vector<std::pair<int, int>> availableCase = AvailableNextCase(current.first, current.second);
        std::pair<int, int> nextCase = GetBestOption(current, availableCase);

        if (std::find(m_AlreadyVistedCaseJPS.begin(), m_AlreadyVistedCaseJPS.end(), nextCase) != m_AlreadyVistedCaseJPS.end()) {
            availableCase.erase(std::find(availableCase.begin(), availableCase.end(), nextCase));
            nextCase = GetBestOption(current, availableCase);
        }

        m_AlreadyVistedCaseJPS.push_back(current);
        m_ResultJPS.push_back(nextCase);
        JPS(nextCase);
    }
   
}



