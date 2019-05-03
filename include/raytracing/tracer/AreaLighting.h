#pragma once

#include "raytracing/tracer/Tracer.h"

namespace rt
{

class AreaLighting : public Tracer
{
public:
	AreaLighting(const World* world);

	virtual RGBColor TraceRay(const Ray& ray) const;

	virtual RGBColor TraceRay(const Ray& ray, int depth) const;

}; // AreaLighting

}
