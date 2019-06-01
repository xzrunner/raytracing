#include "raytracing/primitive/ConcaveHemisphere.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

ConcaveHemisphere::ConcaveHemisphere(const Point3D& center, float radius)
    : m_center(center)
    , m_radius(radius)
{
}

bool ConcaveHemisphere::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
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

			Vector3D hit = ray.ori + t * ray.dir - m_center;

			if (hit.y > m_center.y )
			{
				tmin = t;
				sr.normal = - (temp + t * ray.dir) / m_radius;   // points outwards
				sr.local_hit_point = ray.ori + tmin * ray.dir;
				return (true);
			}
			/*
			tmin = t;
			sr.normal 	 = -(temp + t * ray.dir) / radius;
			sr.local_hit_point = ray.ori + t * ray.dir;
			return (true);*/
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {

			Vector3D hit = ray.ori + t * ray.dir - m_center;

			if (hit.y > m_center.y )
			{
				tmin = t;
				sr.normal = - (temp + t * ray.dir) / m_radius;   // points outwards
				sr.local_hit_point = ray.ori + tmin * ray.dir;
				return (true);
			}
			/*
			tmin = t;
			sr.normal   = -(temp + t * ray.dir) / radius;
			sr.local_hit_point = ray.ori + t * ray.dir;
			return (true);
			*/
		}
	}

	return (false);
}

bool ConcaveHemisphere::ShadowHit(const Ray& ray, float& tmin) const
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
			Vector3D hit = ray.ori + t * ray.dir - m_center;
			if ( t < tmin && hit.y > 0 )
			{
				tmin = static_cast<float>(t);
				return (true);
			}
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {
			Vector3D hit = ray.ori + t * ray.dir - m_center;
			if ( t < tmin && hit.y > 0)
			{
				tmin = static_cast<float>(t);
				return (true);
			}
		}
	}

	return (false);
}

}