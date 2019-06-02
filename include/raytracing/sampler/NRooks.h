#pragma once

#include "raytracing/sampler/Sampler.h"

namespace rt
{

class NRooks : public Sampler
{
public:
    NRooks(int num);

    virtual void GenerateSamples() override;

}; // NRooks

}