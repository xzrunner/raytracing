#pragma once

#include "raytracing/objects/Compound.h"

namespace rt
{

class SolidCone : public Compound
{
public:
    SolidCone();
    SolidCone(double radius, double height);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;
    virtual bool ShadowHit(const Ray& ray, float& t) const override;

    virtual AABB GetBoundingBox() const override { return bbox; }

private:
    AABB bbox;	// the bounding box

}; // SolidCone

}