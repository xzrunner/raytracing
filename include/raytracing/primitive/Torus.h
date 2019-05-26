#pragma once

#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/maths/AABB.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

class Torus : public GeoPrimitive
{
public:
    Torus();
    Torus(double swept_radius, double tube_radius);

    virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
    virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

private:
    Normal ComputeNormal(const Point3D& p) const;

private:
    double m_a;	 	// swept radius
    double m_b;	 	// tube radius
    AABB   m_bbox;	// the bounding box

}; // Torus

}