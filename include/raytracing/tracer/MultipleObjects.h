#pragma once

#include "raytracing/tracer/Tracer.h"

namespace rt
{

class World;

class MultipleObjects : public Tracer
{
public:
	MultipleObjects(const World& world);

	virtual RGBColor TraceRay(const Ray& ray) const;

}; // MultipleObjects

}
