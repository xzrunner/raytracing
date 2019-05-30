#include "raytracing/tracer/PathTrace.h"
#include "raytracing/world/World.h"
#include "raytracing/material/Material.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

PathTrace::PathTrace(const World& world)
	: Tracer(world)
{
}

RGBColor PathTrace::TraceRay(const Ray& ray, int depth) const
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
			return sr.material->Shade(sr);
		} else {
            return m_world.GetBackgroundColor();
        }
	}
}

RGBColor PathTrace::TraceRay(const Ray& ray, double& tmin, int depth) const
{
    if (depth > m_world.GetViewPlane().GetMaxDepth())
    {
        tmin = HUGE_VALUE;
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

			return sr.material->Shade(sr);
		}
		else
        {
			tmin = HUGE_VALUE;
			return m_world.GetBackgroundColor();
		}
	}
}

}