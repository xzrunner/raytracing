#pragma once

#include "raytracing/tracer/Tracer.h"

namespace rt
{

 // todo same to RayCast
class FirstHit : public Tracer
{
public:
    FirstHit(const World& world);

    virtual RGBColor TraceRay(const Ray& ray) const override;

    virtual RGBColor TraceRay(const Ray& ray, int depth) const override;

}; // FirstHit

}