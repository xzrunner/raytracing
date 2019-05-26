#include "raytracing/primitive/ConcaveSphere.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/utilities/Constants.h"
#include "raytracing/utilities/ShadeRec.h"

namespace rt
{

ConcaveSphere::ConcaveSphere(Point3D center, double r)
    : m_center(center)
    , m_radius(r)
{
}

bool ConcaveSphere::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double 		t;
	Vector3D	temp 	= ray.ori - m_center;
	double 		a 		= ray.dir * ray.dir;
	double 		b 		= 2.0 * temp * ray.dir;
	double 		c 		= temp * temp - m_radius * m_radius;
	double 		disc	= b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {
			tmin = t;
			sr.normal 	 = -(temp + t * ray.dir) / m_radius;
			sr.local_hit_point = ray.ori + t * ray.dir;
			return (true);
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {
			tmin = t;
			sr.normal   = -(temp + t * ray.dir) / m_radius;
			sr.local_hit_point = ray.ori + t * ray.dir;
			return (true);
		}
	}

	return (false);
}

bool ConcaveSphere::ShadowHit(const Ray& ray, float& tmin) const
{
    if (!m_shadows) {
        return false;
    }

	double 		t;
	Vector3D  	temp 	= ray.ori - m_center;
	double 		a 		= ray.dir * ray.dir;
	double 		b 		= 2.0 * temp * ray.dir;
	double 		c 		= temp * temp - m_radius * m_radius;
	double 		disc 	= b * b - 4.0 * a * c;

	if (disc < 0.0)
		return(false);
	else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {
			if ( t < tmin )
			{
				tmin = t;
				return (true);
			}
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {
			if ( t < tmin )
			{
				tmin = t;
				return (true);
			}
		}

        return false;
	}
}

}