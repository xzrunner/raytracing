#pragma once

#include "raytracing/samplers/Sampler.h"

namespace rt
{

class MultiJittered : public Sampler
{
public:
	MultiJittered(int num);

	virtual void GenerateSamples();

}; // MultiJittered

}
