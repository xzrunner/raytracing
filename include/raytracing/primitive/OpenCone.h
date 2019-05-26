#pragma once

#include "raytracing/primitive/GeoPrimitive.h"

namespace rt
{

class OpenCone : public GeoPrimitive
{
public:
    OpenCone(double height, double radius);

    virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
    virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

private:
    double m_h = 2.0;
    double m_r = 1.0;

}; // OpenCone

}