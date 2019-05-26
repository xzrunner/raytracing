#pragma once

#include "raytracing/sampler/Sampler.h"

namespace rt
{

class PureRandom : public Sampler
{
public:
	PureRandom(int num);

	virtual void GenerateSamples();

}; // PureRandom

}
