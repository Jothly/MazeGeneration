#include "AltitudeFiltering.h"
#include "RandomGenerator.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>


float lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}


Color32 lerp(Color32 a, Color32 b, float t)
{
	return Color32(
		lerp(a.r, b.r, t),
		lerp(a.g, b.g, t),
		lerp(a.b, b.b, t)
	);
}


float getNormalizedColorValue(Color32 col)
{
	float v = (col.r + col.g + col.b) / 3.0;
	return std::clamp(v / 255.0, 0.0, 1.0);
}


std::vector<Color32> AltitudeFiltering::Generate(int sideSize, float displacement)
{
	using namespace std;

	const Color32 WATER_COLOR = Color32(0, 21, 255);
	const Color32 SAND_COLOR = Color32(222, 204, 69);
	const Color32 GRASS_COLOR = Color32(28, 199, 6);
	const Color32 ROCK_COLOR = Color32(117, 56, 21);
	const Color32 SNOW_COLOR = Color32(255, 255, 255);

	// Note: assumes pairs are ordered by float value in ascending order,
	// and assumes first element is 0 and last element is 1
	std::vector<pair<float, Color32>> colorGradients = // float between 0 and 1 (0 and 255)
	{
		{0, WATER_COLOR}, 
		{.15, WATER_COLOR},
		{.2, SAND_COLOR},
		{.22, SAND_COLOR}, 
		{.27, GRASS_COLOR},
		{.35, GRASS_COLOR},
		{.4, ROCK_COLOR},
		{.425, ROCK_COLOR},
		{.5, SNOW_COLOR}, 
		{1, SNOW_COLOR}
	};

	RandomScenarioGenerator randScenarioGenerator;
	std::vector<Color32> pixelColors = randScenarioGenerator.Generate(sideSize, displacement);

	float max = 0;
	for (Color32& col : pixelColors)
	{
		if (col.r / 255.0 > max) max = col.r / 255.0;
	}

	std::cout << max << std::endl;

	for (Color32& col : pixelColors)
	{
		// Get normalized color value between 0 and 1
		float v = getNormalizedColorValue(col);

		bool success = false;

		for (int i = 1; i < colorGradients.size(); i++)
		{
			if (v <= colorGradients[i].first)
			{
				pair<float, Color32> firstPair = colorGradients[i - 1];
				pair<float, Color32> secondPair = colorGradients[i];

				float portion = (v - firstPair.first) / (secondPair.first - firstPair.first);

				col = lerp(firstPair.second, secondPair.second, portion);

				success = true;

				break;
			}
		}

		if (success == false) throw exception("Altitude Filtering color gradient failed, check colorGradients list");
	}

	return pixelColors;
}
