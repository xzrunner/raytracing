#include "raytracing/primitive/PartAnnulus.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/Normal.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

PartAnnulus::PartAnnulus()
    : bbox(-1, 1, -EPSILON, EPSILON, -1, 1)
{
}

PartAnnulus::PartAnnulus(Point3D c, Normal n, double i, double w, double mi, double ma)
	: center(c)
    , normal(n)
    , inner_radius(i)
    , wall_thickness(w)
    , min_azimuth(mi * PI_ON_180)
    , max_azimuth(ma * PI_ON_180)
    , bbox( center.x - i - w, center.x + i + w, center.y - EPSILON, center.y + EPSILON, center.z - i - w, center.z + i + w)
    , i_squared(i * i)
    , w_squared((i + w) * (i + w))
{
}

bool PartAnnulus::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	if (!bbox.Hit(ray))
		return (false);

	double t = (center - ray.ori) * normal / (ray.dir * normal);

	if (t <= EPSILON)
		return (false);

	Point3D p = ray.ori + t * ray.dir;

	double v = center.DistanceSquared(p);

	if ( v < w_squared && v > i_squared ) {

		double phi = atan2(p.x, p.z);
		if (phi < 0.0)
			phi += TWO_PI;

		if( phi <= max_azimuth && phi >= min_azimuth )
		{
			tmin 				= t;
			sr.normal 			= normal;
			sr.local_hit_point	= p;
			return (true);
		}
		else
			return false;
	}
	else
		return (false);
}

bool PartAnnulus::ShadowHit(const Ray& ray, float& tmin) const
{
    if (!m_shadows || !bbox.Hit(ray)) {
        return false;
    }

	double t = (center - ray.ori) * normal / (ray.dir * normal);

	if (t <= EPSILON)
		return (false);

	Point3D p = ray.ori + t * ray.dir;

	double v = center.DistanceSquared(p);

	if ( v < w_squared && v > i_squared ) {

		double phi = atan2(p.x, p.z);
		if (phi < 0.0)
			phi += TWO_PI;

		if( phi <= max_azimuth && phi >= min_azimuth )
		{
			tmin = static_cast<float>(t);
			return true;
		}
		else
			return false;
	}
	else
		return (false);
}

}