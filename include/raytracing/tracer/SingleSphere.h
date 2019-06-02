#pragma once

#include "raytracing/tracer/Tracer.h"

namespace rt
{

class SingleSphere : public Tracer
{
public:
    SingleSphere(const World& world);

    virtual RGBColor TraceRay(const Ray& ray) const override;

}; // SingleSphere

}