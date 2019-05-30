#pragma once

#include "raytracing/primitive/Compound.h"
#include "raytracing/maths/AABB.h"

namespace rt
{

class ConcaveLens : public Compound
{
public:
    ConcaveLens(float radius, float thickness,
        float min_distance);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;

    virtual AABB GetBoundingBox() const override { return bbox; }

private:
    void BuildComponents();

private:
    float lens_radius  = 0;
    float thickness    = 0;
    float min_distance = 0;
    AABB  bbox;

}; // ConcaveLens

}
