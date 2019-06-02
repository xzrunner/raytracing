#pragma once

#include "raytracing/primitive/GeoPrimitive.h"

namespace rt
{

class CutFace : public GeoPrimitive
{
public:
    CutFace() {}
    CutFace(double size, double radius);

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;
    virtual bool ShadowHit(const Ray& ray, float& t) const override;

private:
    double m_size   = 1;		// x and w dimensions of the square
    double m_radius = 0.5;		// radius of the hole

}; // CutFace

}