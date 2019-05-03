#include "raytracing/tracer/RayCast.h"
#include "raytracing/world/World.h"
#include "raytracing/materials/Material.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

RayCast::RayCast(const World* world)
	: Tracer(world)
{}

RGBColor RayCast::TraceRay(const Ray& ray) const
{
	ShadeRec sr(m_world->HitObjects(ray));
	if (sr.hit_an_object) {
		sr.ray = ray;
		return sr.material->Shade(sr);
	} else {
		return m_world->GetBackgroundColor();
	}
}

// this ignores the depth argument
RGBColor RayCast::TraceRay(const Ray& ray, int depth) const
{
	return TraceRay(ray);
}

}