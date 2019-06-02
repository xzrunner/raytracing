#pragma once

#include "raytracing/primitive/Compound.h"

namespace rt
{

class BeveledBoxShell : public Compound
{
public:
    BeveledBoxShell(const Point3D& bottom, const Point3D& top, float bevel_radius);

    virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
    virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

private:
    AABB		bbox;	// the bounding box

    Point3D p0;
    Point3D p1;
    float br = 0;

}; // BeveledBoxShell

}