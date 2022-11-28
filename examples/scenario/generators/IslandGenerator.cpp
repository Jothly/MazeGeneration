#include "IslandGenerator.h"
#include "RandomGenerator.h"
#include "AltitudeFiltering.h"
#include <iostream>
#include "Point2D.h"
#include <algorithm>


std::vector<Color32> IslandGenerator::Generate(int sideSize, float displacement)
{
	const float BASE_ALTITUDE = 1.1F;
	const float ISLAND_SIZE_MODIFIER = 1.2;

	RandomScenarioGenerator randScenarioGenerator;
	std::vector<Color32> pixelColors = randScenarioGenerator.Generate(sideSize, displacement);

	std::cout << pixelColors.size() << std::endl;

	Point2D center = Point2D(sideSize / 2, sideSize / 2);

	for (int i = 0; i < pixelColors.size(); i++)
	{
		Point2D coord = Point2D(i % sideSize, i / sideSize);

		Point2D vecBetween = coord - center;
		
		float dist = sqrt(vecBetween.x * vecBetween.x + vecBetween.y * vecBetween.y);

		float modifier = BASE_ALTITUDE - std::clamp(dist / (center.x * ISLAND_SIZE_MODIFIER), 0.0F, 1.0F);

		Color32 col = pixelColors[i];
		pixelColors[i] = Color32(
			col.r * modifier,
			col.g * modifier,
			col.b * modifier
		);
	}

	AltitudeFiltering altitudeFilter = AltitudeFiltering();

	return altitudeFilter.Generate(pixelColors);
}
