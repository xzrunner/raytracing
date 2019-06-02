#include "raytracing/tracer/SingleSphere.h"
#include "raytracing/primitive/Sphere.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace
{

rt::Sphere sp;

}

namespace rt
{

SingleSphere::SingleSphere(const World& world)
    : Tracer(world)
{
}

RGBColor SingleSphere::TraceRay(const Ray& ray) const
{
	ShadeRec	sr(m_world); 	// not used
	double    	t;  				// not used

    // todo init shpere
//	if (m_world->sphere.Hit(ray, t, sr))
    if (sp.Hit(ray, t, sr))
		return RED;
	else
		return BLACK;
}

}