#pragma once

#include "raytracing/objects/Object.h"

namespace rt
{

class ConcaveSphere : public Object
{
public:
    ConcaveSphere() {}
    ConcaveSphere(Point3D center, double r);

    virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
    virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

    void SetShadows(bool shadows) { m_shadows = shadows; }

private:
	Point3D m_center;   			// center coordinates as a point
	double 	m_radius = 1.0;		// the radius

	bool m_shadows = false;

}; // ConcaveSphere

}