#include "HuntAndKillMazeGenerator.h"
#include <iostream>


// Checks if point is within bounds of map
bool HuntAndKillMazeGenerator::isPointInMap(Point2D point, int mazeHalfSize)
{
	if (point.y > mazeHalfSize || point.y < -mazeHalfSize || point.x > mazeHalfSize || point.x < -mazeHalfSize)
	{
		return false;
	}

	return true;
}


HuntAndKillMazeGenerator::HuntAndKillMazeGenerator()
{

}


bool HuntAndKillMazeGenerator::Step(World* world)
{
	int mazeHalfSize = (world->GetSize() - 1) / 2;

	if (visitedNodes.empty())
	{
		// Note: Upperbound is inclusivve
		currentNode = Point2D(Random::Range(-mazeHalfSize, mazeHalfSize), Random::Range(-mazeHalfSize, mazeHalfSize));
		visitedNodes.insert(currentNode);
		displayColors(world, mazeHalfSize, false, true);
		return true;
	}

	if (hunting) // Need to hunt for another node
	{
		std::pair<Point2D, bool> huntResult = huntThroughNextRow(mazeHalfSize);

		if (huntResult.second == true)
		{
			currentNode = huntResult.first;
			hunting = false;;
			displayColors(world, mazeHalfSize, true, true);
		}
		else
		{
			if (completedRowNum - mazeHalfSize <= mazeHalfSize)
			{
				displayColors(world, mazeHalfSize, true, false);
			}
			else // Finished
			{
				hunting = false;
				displayColors(world, mazeHalfSize, false, false);
				return false;
			}
		}
	}
	else // Not hunting, do the depth first random
	{
		bool successful = advanceNode(world, mazeHalfSize);

		if (successful == false)
		{
			hunting = true;
		}

		displayColors(world, mazeHalfSize, false, true);

		return true;
	}

	return false;
}


void HuntAndKillMazeGenerator::Clear(World* world)
{
	visitedNodes.clear();

	hunting = false;
	completedRowNum = 0;
}


// Returns found point and a bool telling if 
std::pair<Point2D, bool> HuntAndKillMazeGenerator::huntThroughNextRow(int mazeHalfSize)
{
	// Wrap below code in this if statement if we don't want to be able to step through
	/*while (completedRowNum - mazeHalfSize <= mazeHalfSize)
	{}*/
	
	for (int x = -mazeHalfSize; x <= mazeHalfSize; x++)
	{
		Point2D posPoint = Point2D(x, completedRowNum - mazeHalfSize);

		for (int i = 0; i < 4; i++)
		{
			Point2D neighbor = posPoint - pointMods[i];
			if (visitedNodes.find(neighbor) != visitedNodes.end())
			{
				return std::pair<Point2D, bool>(posPoint, true);;
			}
		}
	}

	completedRowNum++;

	if (completedRowNum - mazeHalfSize <= mazeHalfSize)
	{
		return std::pair<Point2D, bool>(Point2D(), true);
	}
	else // Finished search, maze is complete
	{
		return std::pair<Point2D, bool>(Point2D(), false);
	}
}


bool HuntAndKillMazeGenerator::advanceNode(World* world, int mazeHalfSize)
{
	int pointModsSize = 4;

	while (pointModsSize > 0)
	{
		int currentPointModIndex = Random::Range(0, pointModsSize - 1);
		Point2D posNextNode = currentNode - pointMods[currentPointModIndex];

		std::cout << "POS NEXT NODE: " << posNextNode.x << " , " << posNextNode.y << std::endl;

		if (isPointInMap(posNextNode, mazeHalfSize) && visitedNodes.find(posNextNode) == visitedNodes.end())
		{
			connectNodes(currentNode, posNextNode, world);
			currentNode = posNextNode;
			visitedNodes.insert(currentNode);
			std::cout << "SUCCESS!!!!" << std::endl;
			return true;
		}

		pointModsSize--;

		Point2D temp = pointMods[currentPointModIndex];
		pointMods[currentPointModIndex] = pointMods[pointModsSize];
		pointMods[pointModsSize] = temp;
	}

	return false;
}


bool HuntAndKillMazeGenerator::connectNodes(Point2D p1, Point2D p2, World* world)
{
	Point2D fromP1ToP2 = p2 - p1;

	if (fromP1ToP2.x == -1)
	{
		world->SetWest(p1, false);
		world->SetEast(p2, false);
		return true;
	}
	else if (fromP1ToP2.x == 1)
	{
		world->SetEast(p1, false);
		world->SetWest(p2, false);
		return true;
	}
	else if (fromP1ToP2.y == -1)
	{
		world->SetNorth(p1, false);
		world->SetSouth(p2, false);
		return true;
	}
	else if (fromP1ToP2.y == 1)
	{
		world->SetSouth(p1, false);
		world->SetNorth(p2, false);
		return true;
	}

    return false;
}


void HuntAndKillMazeGenerator::displayColors(World* world, int mazeHalfSize, bool huntingCompletedRow, bool shouldDrawCurrentPoint)
{
	for (Point2D p : visitedNodes)
	{
		world->SetNodeColor(p, Color32(0, 0, 0));
	}

	if (hunting)
	{
		for (int x = -mazeHalfSize; x <= mazeHalfSize; x++)
		{
			int y = completedRowNum - huntingCompletedRow;
			world->SetNodeColor(Point2D(x, y), Color32(255, 255, 0));
		}
	}
	
	if(shouldDrawCurrentPoint)
	{
		world->SetNodeColor(currentNode, Color32(50, 255, 100));
	}
}