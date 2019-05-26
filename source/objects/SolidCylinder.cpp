#include "raytracing/objects/SolidCylinder.h"
#include "raytracing/objects/Disk.h"
#include "raytracing/objects/OpenCylinder.h"

namespace rt
{

SolidCylinder::SolidCylinder()
{
	//This is new function for chapter 30 for any diffault para solid cylinders. Ex 30.12
    // bottom
	m_parts.push_back(std::make_shared<Disk>(Point3D(0, -1, 0), Normal(0, -1, 0), 1));
    // top
    m_parts.push_back(std::make_shared<Disk>(Point3D(0, 1, 0), Normal(0, 1, 0), 1));
    // wall
    m_parts.push_back(std::make_shared<OpenCylinder>(-1, 1, 1));

	bbox.x0 = -1;
	bbox.y0 = -1;
	bbox.z0 = -1;
	bbox.x1 = 1;
	bbox.y1 = 1;
	bbox.z1 = 1;
}

SolidCylinder::SolidCylinder(float bottom, float top, float radius)
{
    // bottom
	m_parts.push_back(std::make_shared<Disk>(Point3D(0, bottom, 0), Normal(0, -1, 0), radius));
    // top
    m_parts.push_back(std::make_shared<Disk>(Point3D(0, top, 0), Normal(0, 1, 0), radius));
    // wall
    m_parts.push_back(std::make_shared<OpenCylinder>(bottom, top, radius));

	bbox.x0 = -radius;
	bbox.y0 = bottom;
	bbox.z0 = -radius;
	bbox.x1 = radius;
	bbox.y1 = top;
	bbox.z1 = radius;
}

bool SolidCylinder::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
    if (bbox.Hit(ray)) {
        return Compound::Hit(ray, tmin, sr);
    } else {
        return false;
    }
}

bool SolidCylinder::ShadowHit(const Ray& ray, float& tmin) const
{
    if (m_shadows && bbox.Hit(ray)) {
        return Compound::ShadowHit(ray, tmin);
    } else {
        return false;
    }
}

}