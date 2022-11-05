#include "PrimMazeGenerator.h"
#include <unordered_map>
#include <assert.h>


static enum Direction
{
	NORTH,
	SOUTH,
	EAST,
	WEST
};



// Checks if point is within bounds of map
bool isPointInMap(Point2D point, int mapHalfSize)
{
	if (point.y > mapHalfSize || point.y < -mapHalfSize || point.x > mapHalfSize || point.x < -mapHalfSize)
	{
		return false;
	}

	return true;
}


void addNeighborsToFrontier(std::unordered_set<Point2D, PrimMazeGenerator::Point2DHashFunction>& frontier, 
	const std::unordered_set<Point2D, PrimMazeGenerator::Point2DHashFunction>& processedNodes, 
	Point2D nodePoint, int mazeHalfSize)
{
	auto tryAddToFrontier = [&](Point2D p) // Adds p to frontier if it hasn't already been processed
	{
		if (processedNodes.find(p) == processedNodes.end())
		{
			frontier.insert(p);
		}
	};

	assert(isPointInMap(nodePoint, mazeHalfSize));

	// Check if new point would be out of bounds, and if not try and add it to frontier
	if (nodePoint.y - 1 >= -mazeHalfSize)
	{
		Point2D posPoint = Point2D(nodePoint.x, nodePoint.y - 1);
		tryAddToFrontier(posPoint);
	}

	if (nodePoint.y + 1 <= mazeHalfSize)
	{
		Point2D posPoint = Point2D(nodePoint.x, nodePoint.y + 1);
		tryAddToFrontier(posPoint);
	}

	if (nodePoint.x - 1 >= -mazeHalfSize)
	{
		Point2D posPoint = Point2D(nodePoint.x - 1, nodePoint.y);
		tryAddToFrontier(posPoint);
	}

	if (nodePoint.x + 1 <= mazeHalfSize)
	{
		Point2D posPoint = Point2D(nodePoint.x + 1, nodePoint.y);
		tryAddToFrontier(posPoint);
	}
}


void connectRandomNeighbor(Point2D nodePoint, World* world, int mapHalfSize,
	const std::unordered_set<Point2D, PrimMazeGenerator::Point2DHashFunction>& processedNodes)
{
	// Maps direction to change in position
	std::unordered_map<Direction, Point2D> dirToPointMod =
	{
		{Direction::NORTH, Point2D(0, -1)},
		{Direction::SOUTH, Point2D(0, 1)},
		{Direction::EAST, Point2D(1, 0)},
		{Direction::WEST, Point2D(-1, 0)}
	};

	// Tries to connect neighbor to ogPoint, returns if connect was successful
	auto tryConnectNeighbor = [&](Point2D ogPoint, Direction dirFromOGToNeighbor) -> bool
	{
		// No + override so gotta do something weird to add
		Point2D modPoint = Point2D(0, 0) - dirToPointMod[dirFromOGToNeighbor];
		Point2D neighborPoint = ogPoint - modPoint;

		assert(isPointInMap(neighborPoint, mapHalfSize));

		if (processedNodes.find(neighborPoint) != processedNodes.end())
		{
			switch (dirFromOGToNeighbor)
			{
				case Direction::NORTH:
					world->SetNorth(ogPoint, false);
					world->SetSouth(neighborPoint, false);
					break;

				case Direction::SOUTH:
					world->SetSouth(ogPoint, false);
					world->SetNorth(neighborPoint, false);
					break;

				case Direction::EAST:
					world->SetEast(ogPoint, false);
					world->SetWest(neighborPoint, false);
					break;

				case Direction::WEST:
					world->SetWest(ogPoint, false);
					world->SetEast(neighborPoint, false);
					break;

				default:
					throw std::exception("dirFromOGToNeighbor is invalid");
			}

			return true;
		}

		return false;
	};

	// Holds the remaining options for directions to travel in search of processed neighbor
	Direction possibleDirections[4] = { Direction::NORTH, Direction::SOUTH, Direction::EAST, Direction::WEST };
	int possibleDirectionsSize = 4;

	while (possibleDirectionsSize > 0) // Loop through possibleDirections until we have a successful connection
	{
		// Note: Upperbound inclusive
		int randIndex = Random::Range(0, possibleDirectionsSize - 1);
		Direction randDirection = possibleDirections[randIndex];

		bool successfulConnection = tryConnectNeighbor(nodePoint, randDirection);

		if (successfulConnection)
		{
			return;
		}
		else // Reduce selection range and move invalid direction to back of array so we don't select it again
		{
			possibleDirectionsSize--;

			Direction temp = possibleDirections[randIndex];
			possibleDirections[randIndex] = possibleDirections[possibleDirectionsSize];
			possibleDirections[possibleDirectionsSize] = temp;
		}
	}

	throw std::exception("No valid neighbor, should be impossible");
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
	int mazeHalfSize = (world->GetSize() - 1) / 2;

	if (processedNodes.empty()) // Maze is just starting out and needs initialization
	{
		// Select random starting point
		// Note: Upperbound is inclusive
		Point2D randomPoint = Point2D(Random::Range(-mazeHalfSize, mazeHalfSize), Random::Range(-mazeHalfSize, mazeHalfSize));

		processedNodes.insert(randomPoint);
		addNeighborsToFrontier(frontier, processedNodes, randomPoint, mazeHalfSize);

		displayColors(world);

		return true;
	}

	if (frontier.empty()) return false; // If no nodes in frontier, we're done!

	// https://www.reddit.com/r/cpp_questions/comments/r6fqsb/question_using_rand_to_pick_a_random_in_an/
	// Get random frontier point
	Point2D frontierPoint;
	std::sample(frontier.begin(), frontier.end(), &frontierPoint, 1, randGen);

	processedNodes.insert(frontierPoint);
	frontier.erase(frontierPoint);

	// Process node
	connectRandomNeighbor(frontierPoint, world, mazeHalfSize, processedNodes);
	addNeighborsToFrontier(frontier, processedNodes, frontierPoint, mazeHalfSize);

	displayColors(world);

	return true; 
}


void PrimMazeGenerator::Clear(World* world)
{
	frontier.clear();
	processedNodes.clear();

	initRandomGenerator();
}


void PrimMazeGenerator::displayColors(World* world)
{
	for (Point2D p : frontier)
	{
		world->SetNodeColor(p, Color32(255, 50, 0));
	}

	for (Point2D p : processedNodes)
	{
		world->SetNodeColor(p, Color32(0, 0, 0));
	}
}
