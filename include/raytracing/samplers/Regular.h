#pragma once

#include "raytracing/samplers/Sampler.h"

namespace rt
{

class Regular : public Sampler
{
public:
	Regular(int num);

	virtual void GenerateSamples();

}; // Regular

}
