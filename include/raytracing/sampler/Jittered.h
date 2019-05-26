#pragma once

#include "raytracing/sampler/Sampler.h"

namespace rt
{

class Jittered : public Sampler
{
public:
	Jittered(int num);

	virtual void GenerateSamples();

}; // Jittered

}
