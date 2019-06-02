#pragma once

#include "raytracing/sampler/Sampler.h"

namespace rt
{

class Hammersley : public Sampler
{
public:
	Hammersley(int num);

	virtual void GenerateSamples();

private:
    double Phi(int j);

}; // Hammersley

}
