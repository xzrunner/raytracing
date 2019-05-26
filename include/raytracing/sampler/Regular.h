#pragma once

#include "raytracing/sampler/Sampler.h"

namespace rt
{

class Regular : public Sampler
{
public:
	Regular(int num);

	virtual void GenerateSamples();

}; // Regular

}
