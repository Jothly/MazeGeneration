#pragma once

#include <unordered_set>
#include <unordered_map>
#include "../MazeGeneratorBase.h"
#include "../Point2DHashFunction.h"
#include "../World.h"
#include "Point2D.h"
#include "Random.h"


class HuntAndKillMazeGenerator : public MazeGeneratorBase
{
	// Maps direction to change in position
	Point2D pointMods[4] =
	{
		Point2D(0, -1),
		Point2D(0, 1),
		Point2D(1, 0),
		Point2D(-1, 0)
	};


	bool hunting = false;

	int completedRowNum = 0;

	Point2D currentNode = Point2D();
	std::unordered_set<Point2D, Point2DHashFunction> visitedNodes;

	std::pair<Point2D, bool> huntThroughNextRow(int mazeHalfSize, World* world);
	bool advanceNode(World* world, int mazeHalfSide);

	bool isPointInMap(Point2D point, int mazeHalfSize);

	bool connectNodes(Point2D p1, Point2D p2, World* world);

	void displayColors(World* world, int mazeHalfSize, bool shouldDrawCurrentPoint, bool shouldDrawHuntingLine, bool huntingCompletedRow = false);

	public:
	HuntAndKillMazeGenerator();

	bool Step(World* world) override;
	void Clear(World* world) override;

	std::string GetName() override { return "Hunt and Kill"; };
};