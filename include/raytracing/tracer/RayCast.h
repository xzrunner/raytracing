#pragma once

#include "raytracing/tracer/Tracer.h"

namespace rt
{

class World;

class RayCast : public Tracer
{
public:
	RayCast(const World* world);

	virtual RGBColor TraceRay(const Ray& ray) const;

	virtual RGBColor TraceRay(const Ray& ray, int depth) const;

}; // RayCast

}
