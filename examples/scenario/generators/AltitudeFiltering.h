#pragma once

#include "../GeneratorBase.h"


class AltitudeFiltering : public ScenarioGeneratorBase
{
	public:

	const Color32 WATER_COLOR = Color32(0, 21, 255);
	const Color32 SAND_COLOR = Color32(222, 204, 69);
	const Color32 GRASS_COLOR = Color32(20, 170, 6);
	const Color32 ROCK_COLOR = Color32(117, 56, 21);
	const Color32 SNOW_COLOR = Color32(255, 255, 255);

	protected:

	ScenarioGeneratorBase* generator;

	// Note: assumes pairs are ordered by float value in ascending order,
	// and assumes first element is 0 and last element is 1
	std::vector<std::pair<float, Color32>> colorGradients = // float between 0 and 1 (0 and 255)
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

	public:

	AltitudeFiltering(ScenarioGeneratorBase* _generator = nullptr);

	std::vector<Color32> Generate(int sideSize, float displacement = 0) override;
	std::vector<Color32> Generate(std::vector<Color32>& pixelColors);

	std::string GetName() override { return "AltitudeFiltering"; }

	void setGenerator(ScenarioGeneratorBase* _generator) { generator = _generator; }
	void setColorGradients(std::vector<std::pair<float, Color32>> _colorGradients) { colorGradients = _colorGradients; }
};