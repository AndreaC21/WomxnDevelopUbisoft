#pragma once

class Level;

class PathFinding
{
public:
	PathFinding();
	void Start(Level& level, std::pair<int, int> from, std::pair<int, int> target);

	std::vector<std::pair<int, int>> GetResultJPS() const;

private:
	//Level m_Level;
	std::pair<int, int> m_Start, m_end;
	std::vector<std::pair<int, int>> m_ResultJPS;
	std::vector<std::pair<int, int>> m_AlreadyVistedCaseJPS;

	void JPS(std::pair<int, int> currentCase);
	std::pair<int, int> GetBestOption(std::pair<int, int>, std::vector<std::pair<int, int>>);
	std::vector<std::pair<int, int>> AvailableNextCase(int currentColumn, int currentRow);
};
/*
m_Level is commented because I have commpilation error " PathFinding::Level use a class Level not defined "
I try to solve it but i haven't succeed in time so unfortunately my code for this algo is untested.
*/

