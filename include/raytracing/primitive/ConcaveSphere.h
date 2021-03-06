#pragma once

#include "raytracing/primitive/GeoPrimitive.h"

namespace rt
{

class ConcaveSphere : public GeoPrimitive
{
public:
    ConcaveSphere() {}
    ConcaveSphere(Point3D center, double r);

    virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
    virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

private:
	Point3D m_center;   			// center coordinates as a point
	double 	m_radius = 1.0;		// the radius

}; // ConcaveSphere

}