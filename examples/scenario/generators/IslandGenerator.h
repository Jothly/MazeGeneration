#pragma once

#include "../GeneratorBase.h"


class IslandGenerator : public ScenarioGeneratorBase
{
	// Inherited via ScenarioGeneratorBase
	virtual std::vector<Color32> Generate(int sideSize, float displacement) override;
	virtual std::string GetName() override { return "Island Generator"; };
};