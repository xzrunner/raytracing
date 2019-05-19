#pragma once

#include "raytracing/objects/Object.h"
#include "raytracing/utilities/Constants.h"

namespace rt
{

class ConcavePartCylinder : public Object
{
public:
    ConcavePartCylinder() {}
    // polar_min£¬ polar_max : in degrees measured from top
    ConcavePartCylinder(double bottom, double top, double radius,
        double polar_min, double polar_max);
    ConcavePartCylinder(double bottom, double top, double radius);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;
    virtual bool ShadowHit(const Ray& ray, float& t) const override;

private:
    double y0         = -1.0;       // bottom y value
    double y1         = 1.0;        // top y value
    double radius     = 1.0;        // radius
    double inv_radius = 1.0;        // one over the radius
    double polar_min  = 0.0;
    double polar_max  = TWO_PI;

}; // ConcavePartCylinder

}
