#pragma once

#include "../GeneratorBase.h"


class AltitudeFiltering : public ScenarioGeneratorBase
{
	public:
	std::vector<Color32> Generate(int sideSize, float displacement = 0) override;
	std::string GetName() override { return "AltitudeFiltering"; }
};