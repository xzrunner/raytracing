#include "raytracing/tracer/Tracer.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

Tracer::Tracer(const World& world)
	: m_world(world)
{
}

RGBColor Tracer::TraceRay(const Ray& ray) const
{
	return BLACK;
}

RGBColor Tracer::TraceRay(const Ray& ray, int depth) const
{
	return BLACK;
}

RGBColor Tracer::TraceRay(const Ray& ray, double &t, const int depth) const
{
    return WHITE;
}

}