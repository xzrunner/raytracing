#pragma once

#include "raytracing/tracer/Tracer.h"

namespace rt
{

class World;

class GlobalTrace : public Tracer
{
public:
	GlobalTrace(const World& world);

    virtual RGBColor TraceRay(const Ray& ray, int depth) const override;

    virtual RGBColor TraceRay(const Ray& ray, double& t, int depth) const override;

}; // GlobalTrace

}
