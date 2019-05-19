#pragma once

#include "raytracing/tracer/Tracer.h"

namespace rt
{

class World;

class Whitted : public Tracer
{
public:
	Whitted(const World& world);

    virtual RGBColor TraceRay(const Ray& ray, int depth) const override;

    virtual RGBColor TraceRay(const Ray& ray, double& t, int depth) const override;

}; // Whitted

}
