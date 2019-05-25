#pragma once

#include "raytracing/samplers/Sampler.h"

namespace rt
{

class MultiJittered : public Sampler
{
public:
	MultiJittered(int num);
    MultiJittered(int num_samples, int m);

	virtual void GenerateSamples();

}; // MultiJittered

}
