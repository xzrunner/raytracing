#pragma once

#include "raytracing/samplers/Sampler.h"

namespace rt
{

class PureRandom : public Sampler
{
public:
	PureRandom(int num);

	virtual void GenerateSamples();

}; // PureRandom

}
