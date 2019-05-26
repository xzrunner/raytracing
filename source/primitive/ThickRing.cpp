#include "raytracing/primitive/ThickRing.h"
#include "raytracing/primitive/Annulus.h"
#include "raytracing/primitive/OpenCylinder.h"

namespace rt
{

ThickRing::ThickRing()
{
	m_parts.push_back(std::make_shared<Annulus>(Point3D(0, 0, 0), Normal(0, -1, 0), 0.9, 0.1));
	m_parts.push_back(std::make_shared<Annulus>(Point3D(0, 1, 0), Normal(0, 1, 0), 0.9, 0.1));
	m_parts.push_back(std::make_shared<OpenCylinder>(0, 1, 0.9));	// iwall
	m_parts.push_back(std::make_shared<OpenCylinder>(0, 1, 1.0));	// owall

	bbox.x0 = -1;
	bbox.y0 = -1;
	bbox.z0 = -1;
	bbox.x1 = 1;
	bbox.y1 = 1;
	bbox.z1 = 1;
}

ThickRing::ThickRing(double bottom, double top, double i_radius, double o_radius)
{
	m_parts.push_back(std::make_shared<Annulus>(Point3D(0, bottom, 0), Normal(0, -1,0 ), i_radius, o_radius - i_radius));
	m_parts.push_back(std::make_shared<Annulus>(Point3D(0, top, 0), Normal(0, 1, 0), i_radius, o_radius - i_radius));
	m_parts.push_back(std::make_shared<OpenCylinder>(bottom, top, i_radius));	// iwall
	m_parts.push_back(std::make_shared<OpenCylinder>(bottom, top, o_radius));	// owall

	bbox.x0 = -o_radius;
	bbox.y0 = bottom;
	bbox.z0 = -o_radius;
	bbox.x1 = o_radius;
	bbox.y1 = top;
	bbox.z1 = o_radius;
}

bool ThickRing::Hit(const Ray& ray, double& tmin, ShadeRec& sr) const
{
    if (bbox.Hit(ray)) {
        return Compound::Hit(ray, tmin, sr);
    } else {
        return false;
    }
}

bool ThickRing::ShadowHit(const Ray& ray, float& tmin) const
{
    if (m_shadows && bbox.Hit(ray)) {
        return Compound::ShadowHit(ray, tmin);
    } else {
        return false;
    }
}

}