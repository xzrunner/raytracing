#include "raytracing/primitive/Sphere.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

#include <math.h>

namespace rt
{

Sphere::Sphere()
	: m_radius(1.0f)
{
}

Sphere::Sphere(const Point3D& center, float r)
	: m_center(center)
	, m_radius(r)
{
}

bool Sphere::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double 		t;
	Vector3D	temp 	= ray.ori - m_center;
	double 		a 		= ray.dir * ray.dir;
	double 		b 		= 2.0 * temp * ray.dir;
	double 		c 		= temp * temp - m_radius * m_radius;
	double 		disc	= b * b - 4.0 * a * c;

	if (disc < 0.0) {
		return false;
	} else {
		double e = sqrt(disc);
		double denom = 2.0 * a;
		t = (-b - e) / denom;    // smaller root

		if (t > EPSILON) {
			tmin = t;
			sr.normal = (temp + t * ray.dir) / m_radius;
			sr.local_hit_point = ray.ori + t * ray.dir;
			return true;
		}

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON) {
			tmin = t;
			sr.normal = (temp + t * ray.dir) / m_radius;
			sr.local_hit_point = ray.ori + t * ray.dir;
			return true;
		}
	}

	return false;
}

bool Sphere::ShadowHit(const Ray& ray, float& tmin) const
{
    if (!m_shadows) {
        return false;
    }

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

		if (t > EPSILON)
			return true;

		t = (-b + e) / denom;    // larger root

		if (t > EPSILON)
			return true;
	}

	return false;
}

void Sphere::SetCenter(const Point3D& center)
{
	m_center = center;
}

void Sphere::SetRadius(float r)
{
	m_radius = r;
}

}