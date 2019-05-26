#pragma once

#include "raytracing/primitive/GeoPrimitive.h"
#include "raytracing/maths/Normal.h"

namespace rt
{

class SmoothTriangle : public GeoPrimitive
{
public:
    SmoothTriangle();
    SmoothTriangle(const Point3D& v0, const Point3D& v1, const Point3D& v2);

    virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& sr) const override;
    virtual bool ShadowHit(const Ray& ray, float& tmin) const override;

    virtual AABB GetBoundingBox() const;

    void SetNormals(const Point3D& n0, const Point3D& n1, const Point3D& n2) {
        m_n0 = n0; m_n1 = n1; m_n2 = n2;
    }

private:
    Normal InterpolateNormal(float beta, float gamma) const;

private:
    Point3D	m_v0, m_v1, m_v2;
    Normal	m_n0, m_n1, m_n2;

}; // SmoothTriangle

}