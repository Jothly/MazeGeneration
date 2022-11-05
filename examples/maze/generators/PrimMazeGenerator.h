#pragma once

#include <Random.h>
#include <stack>
#include <unordered_set>
#include <vector>
#include <random>
#include "../MazeGeneratorBase.h"
#include "../Node.h"
#include "../World.h"
#include "Random.h"
#include "Point2D.h"
#include "../Point2DHashFunction.h"


class PrimMazeGenerator : public MazeGeneratorBase
{
	private:
	std::unordered_set<Point2D, Point2DHashFunction> processedNodes;
	std::unordered_set<Point2D, Point2DHashFunction> frontier;

	std::mt19937 randGen;

	void initRandomGenerator();

	void displayColors(World* world);

	public:
	PrimMazeGenerator();

	bool Step(World* world) override;
	void Clear(World* world) override;

	std::string GetName() override { return "Prim's"; }
};