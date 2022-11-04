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


class PrimMazeGenerator : public MazeGeneratorBase
{
	public:
	// https://stackoverflow.com/questions/38554083/how-can-i-use-a-c-unordered-set-for-a-custom-class
	struct Point2DHashFunction
	{
		size_t operator()(const Point2D& point) const
		{
			size_t xHash = std::hash<int>()(point.x);
			size_t yHash = std::hash<int>()(point.y) << 1;
			return xHash ^ yHash;
		}
	};

	private:
	std::unordered_set<Point2D, Point2DHashFunction> processedNodes;
	std::unordered_set<Point2D, Point2DHashFunction> frontier;

	std::mt19937 randGen;

	void initRandomGenerator();

	public:
	std::string GetName() override { return "Prim's"; }

	PrimMazeGenerator();

	bool Step(World* world) override;
	void Clear(World* world) override;
};