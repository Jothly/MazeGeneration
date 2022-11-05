#pragma once
#include "../../core/Point2D.h"


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