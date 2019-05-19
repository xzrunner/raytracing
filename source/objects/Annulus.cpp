#include "raytracing/objects/Annulus.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

Annulus::Annulus()
    : bbox(-1, 1, -EPSILON, EPSILON, -1, 1)
{
}

Annulus::Annulus(Point3D center, Normal normal, double i, double w)
    : center(center)
    , normal(normal)
    , inner_radius(inner_radius)
    , wall_thickness(wall_thickness)
    , i_squared(i * i)
    , w_squared((i + w) * (i + w))
    , bbox(center.x - i - w, center.x + i + w, center.y - EPSILON, center.y + EPSILON, center.z - i - w, center.z + i + w)
{
}

bool Annulus::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	if (!bbox.Hit(ray))
		return (false);

	double t = (center - ray.ori) * normal / (ray.dir * normal);

	if (t <= EPSILON)
		return (false);

	Point3D p = ray.ori + t * ray.dir;

	double v = center.DistanceSquared(p);

	if ( v < w_squared && v > i_squared ) {
		tmin 				= t;
		sr.normal 			= normal;
		sr.local_hit_point	= p;
		return (true);
	}
	else
		return (false);
}

bool Annulus::ShadowHit(const Ray& ray, float& tmin) const
{
	if (!bbox.Hit(ray))
		return (false);

	double t = (center - ray.ori) * normal / (ray.dir * normal);

	if (t <= EPSILON)
		return (false);

	Point3D p = ray.ori + t * ray.dir;

	double v = center.DistanceSquared(p);

	if ( v < w_squared && v > i_squared ) {
		tmin = static_cast<float>(t);
		return (true);
	}
	else
		return (false);
}

}