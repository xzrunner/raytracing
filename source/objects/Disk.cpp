#include "raytracing/objects/Disk.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/Point2D.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/samplers/Sampler.h"
#include "raytracing/utilities/Constants.h"

namespace
{

const double kEpsilon = 0.001;   // for shadows and secondary rays

}

namespace rt
{

Disk::Disk(const Point3D& point, const Normal& normal, double ra)
    : m_center(point)
    , m_normal(normal)
    , m_r(ra)
    , m_r_squared(ra*ra)
    , m_sampler_ptr(nullptr)
    , m_shadows(false)
    , m_area(static_cast<float>(0.5 * PI * m_r_squared))
    , m_inv_area(1.0f / m_area)
{
    m_normal.Normalize();
}

bool Disk::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t = (m_center - ray.ori) * m_normal / (ray.dir * m_normal);

    if (t <= kEpsilon) {
        return false;
    }

	Point3D p = ray.ori + t * ray.dir;

	if (m_center.DistanceSquared(p) < m_r_squared)
    {
		tmin 				= t;
		sr.normal 			= m_normal;
		sr.local_hit_point	= p;
		return true;
	}
    else
    {
        return false;
    }
}

bool Disk::ShadowHit(const Ray& ray, float& tmin) const
{
	float t = static_cast<float>((m_center - ray.ori) * m_normal / (ray.dir * m_normal));
    if (t <= kEpsilon) {
        return false;
    }

	Point3D p = ray.ori + t * ray.dir;

	if (m_center.DistanceSquared(p) < m_r_squared) {
		tmin = t;
		return true;
	} else {
        return false;
    }
}

Point3D Disk::Sample() const
{
    Point2D sample_point = m_sampler_ptr->SampleUnitDisk();
    return m_center + sample_point.x * m_u + sample_point.y * m_v;
}

}