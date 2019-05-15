#include "raytracing/objects/Triangle.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/utilities/Constants.h"

#include <float.h>
#include <math.h>

namespace rt
{

Triangle::Triangle()
	: m_v0(0, 0, 0)
	, m_v1(0,0,1)
	, m_v2(1,0,0)
	, m_normal(0, 1, 0)
{
}

Triangle::Triangle(const Point3D& v0, const Point3D& v1, const Point3D& v2)
	: m_v0(v0)
	, m_v1(v1)
	, m_v2(v2)
	, m_normal(0, 1, 0)
{
}

bool Triangle::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double a = m_v0.x - m_v1.x, b = m_v0.x - m_v2.x, c = ray.dir.x, d = m_v0.x - ray.ori.x;
	double e = m_v0.y - m_v1.y, f = m_v0.y - m_v2.y, g = ray.dir.y, h = m_v0.y - ray.ori.y;
	double i = m_v0.z - m_v1.z, j = m_v0.z - m_v2.z, k = ray.dir.z, l = m_v0.z - ray.ori.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom  = 1.0 / (a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return false;

	double r = r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0 )
		return false;

	if (beta + gamma > 1.0)
		return false;

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < EPSILON)
		return false;

	tmin 				= t;
	sr.normal 			= m_normal;
	sr.local_hit_point 	= ray.ori + t * ray.dir;

	return true;
}

bool Triangle::ShadowHit(const Ray& ray, float& tmin) const
{
	double a = m_v0.x - m_v1.x, b = m_v0.x - m_v2.x, c = ray.dir.x, d = m_v0.x - ray.ori.x;
	double e = m_v0.y - m_v1.y, f = m_v0.y - m_v2.y, g = ray.dir.y, h = m_v0.y - ray.ori.y;
	double i = m_v0.z - m_v1.z, j = m_v0.z - m_v2.z, k = ray.dir.z, l = m_v0.z - ray.ori.z;

	double m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
	double q = g * i - e * k, s = e * j - f * i;

	double inv_denom  = 1.0 / (a * m + b * q + c * s);

	double e1 = d * m - b * n - c * p;
	double beta = e1 * inv_denom;

	if (beta < 0.0)
		return false;

	double r = r = e * l - h * i;
	double e2 = a * n + d * q + c * r;
	double gamma = e2 * inv_denom;

	if (gamma < 0.0 )
		return false;

	if (beta + gamma > 1.0)
		return false;

	double e3 = a * p - b * r + d * s;
	double t = e3 * inv_denom;

	if (t < EPSILON)
		return false;

	tmin = static_cast<float>(t);

	return true;
}

AABB Triangle::GetBoundingBox() const
{
	float delta = 0.000001f;
	return (AABB(std::min(std::min(m_v0.x, m_v1.x), m_v2.x) - delta, std::max(std::max(m_v0.x, m_v1.x), m_v2.x) + delta,
				 std::min(std::min(m_v0.y, m_v1.y), m_v2.y) - delta, std::max(std::max(m_v0.y, m_v1.y), m_v2.y) + delta,
				 std::min(std::min(m_v0.z, m_v1.z), m_v2.z) - delta, std::max(std::max(m_v0.z, m_v1.z), m_v2.z) + delta));
}

}