#pragma once

#include "raytracing/primitive/GeoPrimitive.h"

#include <memory>

namespace rt
{

class ConcavePartSphere;
class Sphere;

class CutCube : public GeoPrimitive
{
public:
    CutCube();
    CutCube(const Point3D& p0, const Point3D& p1, float radius);
    virtual ~CutCube();

    virtual bool Hit(const Ray& ray, double& t, ShadeRec& s) const override;
    virtual bool ShadowHit(const Ray& ray, float& t) const override;

    virtual AABB GetBoundingBox() const override;

private:
    Normal GetNormal(int face_hit) const;

private:
    Point3D p0;
    Point3D p1;

    std::unique_ptr<ConcavePartSphere> sphere = nullptr;
    std::unique_ptr<Sphere> sp = nullptr;

}; // CutCube

}