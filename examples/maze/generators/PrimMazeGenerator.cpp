#include "PrimMazeGenerator.h"
#include <unordered_map>


static enum Direction
{
	NORTH,
	SOUTH,
	EAST,
	WEST
};


// Assumes nodePoint is valid
void addNeighborsToFrontier(std::unordered_set<Point2D, PrimMazeGenerator::Point2DHashFunction>& frontier, 
	const std::unordered_set<Point2D, PrimMazeGenerator::Point2DHashFunction>& processedNodes, 
	Point2D nodePoint, int mazeSideSize)
{
	auto tryAddToFrontier = [&](Point2D p)
	{
		if (processedNodes.find(p) == processedNodes.end())
		{
			frontier.insert(p);
		}
	};


	if (nodePoint.y - 1 < -mazeSideSize) // Check not out of bounds
	{
		Point2D posPoint = Point2D(nodePoint.x, nodePoint.y - 1);
		tryAddToFrontier(posPoint);
	}

	if (nodePoint.y + 1 > mazeSideSize)
	{
		Point2D posPoint = Point2D(nodePoint.x, nodePoint.y + 1);
		tryAddToFrontier(posPoint);
	}

	if (nodePoint.x - 1 < -mazeSideSize)
	{
		Point2D posPoint = Point2D(nodePoint.x - 1, nodePoint.y);
		tryAddToFrontier(posPoint);
	}

	if (nodePoint.x + 1 > mazeSideSize)
	{
		Point2D posPoint = Point2D(nodePoint.x + 1, nodePoint.y);
		tryAddToFrontier(posPoint);
	}
}


void connectRandomNeighbor(Point2D nodePoint, World* world,
	const std::unordered_set<Point2D, PrimMazeGenerator::Point2DHashFunction>& processedNodes)
{
	std::unordered_map<Direction, Point2D> dirToPointMod =
	{
		{Direction::NORTH, Point2D(0, -1)},
		{Direction::SOUTH, Point2D(0, 1)},
		{Direction::EAST, Point2D(1, 0)},
		{Direction::WEST, Point2D(-1, 0)}
	};

	// Returns if connect was successful
	auto tryConnectNeighbor = [&](Point2D ogPoint, Direction dirFromOGToNeighbor) -> bool
	{
		// No + override so gotta do something weird to add
		Point2D modPoint = Point2D(0, 0) - dirToPointMod[dirFromOGToNeighbor];
		Point2D neighborPoint = ogPoint - modPoint;

		if (processedNodes.find(neighborPoint) == processedNodes.end())
		{
			switch (dirFromOGToNeighbor)
			{
				case Direction::NORTH:
					world->SetNorth(ogPoint, true);
					world->SetSouth(neighborPoint, true);
					break;

				case Direction::SOUTH:
					world->SetSouth(ogPoint, true);
					world->SetNorth(neighborPoint, true);
					break;

				case Direction::EAST:
					world->SetEast(ogPoint, true);
					world->SetWest(neighborPoint, true);
					break;

				case Direction::WEST:
					world->SetWest(ogPoint, true);
					world->SetEast(neighborPoint, true);
					break;

				default:
					throw std::exception();
			}

			return true;
		}

		return false;
	};

	Direction possibleDirections[4] = { Direction::NORTH, Direction::SOUTH, Direction::EAST, Direction::WEST };
	int possibleDirectionsSize = 4;

	while (possibleDirectionsSize > 0)
	{
		// Note: Upperbound inclusive
		int randIndex = Random::Range(0, possibleDirectionsSize - 1);
		Direction randDirection = possibleDirections[randIndex];

		bool successfulConnection = tryConnectNeighbor(nodePoint, randDirection);

		if (successfulConnection)
		{
			return;
		}
		else
		{
			possibleDirectionsSize--;

			Direction temp = possibleDirections[randIndex];
			possibleDirections[randIndex] = possibleDirections[possibleDirectionsSize];
			possibleDirections[possibleDirectionsSize] = temp;
		}
	}

	throw std::exception();
}


// https://www.reddit.com/r/cpp_questions/comments/r6fqsb/question_using_rand_to_pick_a_random_in_an/
void PrimMazeGenerator::initRandomGenerator()
{
	randGen = std::mt19937(std::random_device{}());
}


PrimMazeGenerator::PrimMazeGenerator()
{
	initRandomGenerator();
}


bool PrimMazeGenerator::Step(World* world)
{ 
	int mazeSideSize = world->GetSize();

	if (frontier.empty()) return false;

	if (processedNodes.empty())
	{
		Point2D randomPoint;

		processedNodes.insert(randomPoint);
		addNeighborsToFrontier(frontier, processedNodes, randomPoint, mazeSideSize);
	}

	// https://www.reddit.com/r/cpp_questions/comments/r6fqsb/question_using_rand_to_pick_a_random_in_an/
	// Get random frontier point
	Point2D frontierPoint;
	std::sample(frontier.begin(), frontier.end(), &frontierPoint, 1, randGen);
	processedNodes.insert(frontierPoint);

	connectRandomNeighbor(frontierPoint, world, processedNodes);
	addNeighborsToFrontier(frontier, processedNodes, frontierPoint, world->GetSize());

	return true; 
}


void PrimMazeGenerator::Clear(World* world)
{
	frontier.clear();
	processedNodes.clear();

	initRandomGenerator();
}