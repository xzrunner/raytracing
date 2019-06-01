#pragma once

#include "raytracing/primitive/GeoPrimitive.h"

namespace rt
{

class ConcaveHemisphere : public GeoPrimitive
{
public:
    ConcaveHemisphere() {}
    ConcaveHemisphere(const Point3D& center, float radius);

    virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
    virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

    virtual float Pdf(const ShadeRec& sr) const override { return 0; }

    void SetCenter(const Point3D& center) { m_center = center; }
    void SetRadius(float r) { m_radius = r; }

private:
    Point3D m_center;
    double 	m_radius = 1.0;

}; // ConcaveHemisphere

}