#include "AltitudeFiltering.h"
#include "RandomGenerator.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>


AltitudeFiltering::AltitudeFiltering(ScenarioGeneratorBase* _generator) : generator(_generator)
{}


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

	RandomScenarioGenerator randScenarioGenerator;
	std::vector<Color32> pixelColors = randScenarioGenerator.Generate(sideSize, displacement);

	return Generate(pixelColors);
}


std::vector<Color32> AltitudeFiltering::Generate(std::vector<Color32>& pixelColors)
{
	using namespace std;

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
