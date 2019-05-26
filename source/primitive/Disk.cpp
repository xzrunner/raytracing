#include "raytracing/primitive/Disk.h"
#include "raytracing/maths/Ray.h"
#include "raytracing/maths/Point2D.h"
#include "raytracing/utilities/ShadeRec.h"
#include "raytracing/sampler/Sampler.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

Disk::Disk(const Point3D& point, const Normal& normal, double ra)
    : m_center(point)
    , m_normal(normal)
    , m_r(ra)
    , m_r_squared(ra*ra)
    , m_sampler(nullptr)
    , m_area(static_cast<float>(0.5 * PI * m_r_squared))
    , m_inv_area(1.0f / m_area)
{
    m_normal.Normalize();
}

bool Disk::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
	double t = (m_center - ray.ori) * m_normal / (ray.dir * m_normal);

    if (t <= EPSILON) {
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
    if (!m_shadows) {
        return false;
    }

	float t = static_cast<float>((m_center - ray.ori) * m_normal / (ray.dir * m_normal));
    if (t <= EPSILON) {
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
    Point2D sample_point = m_sampler->SampleUnitDisk();
    return m_center + sample_point.x * m_u + sample_point.y * m_v;
}

void Disk::SetSampler(const std::shared_ptr<Sampler>& sampler)
{
    m_sampler = sampler;
    m_sampler->MapSamplesToUnitDisk();
}

}