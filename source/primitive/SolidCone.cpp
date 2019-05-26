#include "raytracing/primitive/SolidCone.h"
#include "raytracing/primitive/OpenCone.h"
#include "raytracing/primitive/Disk.h"

namespace rt
{

SolidCone::SolidCone()
{
	m_parts.push_back(std::make_shared<OpenCone>(1, 2));

    // top
    m_parts.push_back(std::make_shared<Disk>(Point3D(0, 0, 0), Normal(0, -1, 0), 1));

	bbox.x0 = -1;
	bbox.y0 = 0;
	bbox.z0 = -1;
	bbox.x1 = 1;
	bbox.y1 = 2;
	bbox.z1 = 1;
}

SolidCone::SolidCone(double radius, double height)
{
    m_parts.push_back(std::make_shared<OpenCone>(radius, height));

    // top
    m_parts.push_back(std::make_shared<Disk>(Point3D(0, 0, 0), Normal(0, -1, 0), radius));

	bbox.x0 = -radius;
	bbox.y0 = 0;
	bbox.z0 = -radius;
	bbox.x1 = radius;
	bbox.y1 = height;
	bbox.z1 = radius;
}

bool SolidCone::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
    if (bbox.Hit(ray))
        return (Compound::Hit(ray, tmin, sr));
    else
        return (false);
}

bool SolidCone::ShadowHit(const Ray& ray, float& tmin) const
{
    if (m_shadows && bbox.Hit(ray))
        return (Compound::ShadowHit(ray, tmin));
    else
        return (false);
}

}