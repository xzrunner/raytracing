#include "raytracing/tracer/GlobalTrace.h"
#include "raytracing/world/World.h"
#include "raytracing/materials/Material.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

GlobalTrace::GlobalTrace(const World& world)
	: Tracer(world)
{
}

RGBColor GlobalTrace::TraceRay(const Ray& ray, int depth) const
{
    if (depth > m_world.GetViewPlane().GetMaxDepth())
    {
        return BLACK;
    }
	else
    {
		ShadeRec sr(m_world.HitObjects(ray));

		if (sr.hit_an_object) {
			sr.depth = depth;
			sr.ray   = ray;
			return sr.material->GlobalShade(sr);
		} else {
            return m_world.GetBackgroundColor();
        }
	}
}

RGBColor GlobalTrace::TraceRay(const Ray& ray, double& tmin, int depth) const
{
    if (depth > m_world.GetViewPlane().GetMaxDepth())
    {
        return BLACK;
    }
	else
    {
		ShadeRec sr(m_world.HitObjects(ray));

		if (sr.hit_an_object)
        {
			sr.depth 	= depth;
			sr.ray 		= ray;
			tmin		= sr.t;     // required for colored transparency

			return sr.material->GlobalShade(sr);
		}
		else
        {
			tmin = HUGE_VALUE;
			return m_world.GetBackgroundColor();
		}
	}
}

}